#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>
#include <hdf5.h>
#include "omp.h"

using namespace std;
using namespace arma;



//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path  = "/home/johanna/codes/codes-git/study-paper/trunk/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;





int
main(int argc, char** argv)
{
  
  if(argc < 3 )
  {
    cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
    return -1;
  }
  
  
  int scale_factor = atoi( argv[1] );
  int shift = atoi( argv[2] );
  int dim = 14; 
  int sc =1;
  
  field<string> all_people;
  all_people.load(peopleList);
  
  field<std::string> actions;
  actions.load( actionNames );  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  
  mat uni_features;
  
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    
    mat mat_features_tmp;
    mat mat_features;
    for (int act=0; act<n_actions; ++act)
    {
      mat mat_features_video_i;  
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;	  
      load_folder << path << "dim_" << dim <<"/features/kth-features_dim" << dim <<  "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      mat_features_video_i.load( load_feat_video_i.str(), hdf5_binary );
      //getchar();
      
      if ( mat_features_video_i.n_cols>0 )
      {
	mat_features_tmp	 = join_rows( mat_features_tmp, mat_features_video_i );
	
      }
      else 
      {
	cout << "# vectors = 0 in " << load_feat_video_i.str() << endl;     
	
      }
      
    }
    cout << "r&c "<<  mat_features_tmp.n_rows << " & " << mat_features_tmp.n_cols << endl;
    
    const uword N_max = 10300; //10300x25 = 257.500 ~= 256.000 as per Improved Trajectories Features
    
    if (mat_features_tmp.n_cols > N_max)
    {
      ivec tmp1 = randi( N_max, distr_param(0,mat_features_tmp.n_cols-1) );
      ivec tmp2 = unique(tmp1);
      uvec my_indices = conv_to<uvec>::from(tmp2);
      mat_features = mat_features_tmp.cols(my_indices);  // extract a subset of the columns
      
    }
    
    else
    {
      mat_features = mat_features_tmp;
      
    }
    
    cout << "mat_features.n_cols "<< mat_features.n_cols << endl;
    
    
    if ( mat_features.n_cols>0 )
    {
      uni_features	 = join_rows( uni_features, mat_features );
      
    }
    else 
    {
      cout << "# vectors = 0 in uni_features" << endl;
      
    }
    
    //uni_features =  join_rows( uni_features, mat_features );
    mat_features_tmp.reset();
    mat_features.reset();
    
  }
  
  
  cout << "Final r&c "<<  uni_features.n_rows << " & " << uni_features.n_cols << endl;
  
  
  
  
  
  return 0;
  
}
