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


//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ucf_sports/";
const std::string path_dataset = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";



const std::string  actionNames = "actionNames.txt";


field<std::string> actions;


inline
void
train_ucf(int N_cent,  int dim, int sc);

inline
float
test_ucf(int N_cent, int dim, int sc, int scale_factor, int shift );


inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int pe_ts,  int act);

inline
vec
get_loglikelihoods(mat &mat_features, int run, int N_cent, int dim);




//******************MAIN********************************************************
int
main(int argc, char** argv)
{
  
  int dim = 14; 
  int sc =1;
  
  int N_cent = 256; // as per Improved Trajectories Features
  
  
  //**********Train the mode**********************
  train_ucf( N_cent, dim, sc);
  
  //   //Test
  //   vec vec_shift;  
  //   vec all_acc;
  //  
  //   vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
  //    all_acc.zeros(vec_shift.n_elem);
  //    
  //   for (int i=0; i<vec_shift.n_elem; ++i)
  //   {
  //   int scale_factor =1;
  //   int shift = vec_shift(i);
  //   all_acc(i) = test_ucf( N_cent, dim, sc, scale_factor, shift );
  //   }
  //   
  //   vec_shift.t().print();
  //   all_acc.t().print();
  
  
  return 0;
}


//*************************************************************
//***************************Train******************************

inline
void
train_ucf(int N_cent, int dim, int sc)
{
  actions.load( actionNames ); 
  
  int scale_factor =1;
  int shift = 0;
  int n_actions = actions.n_rows;
  
  int k=0;
  field <std::string> action_seq_names (150-1,3);  ///From the dataset description (150)
  
  
  //Creating list with all videos
  for (int act=0; act<n_actions; ++act)
  {
    std::stringstream  ss_folders;
    ss_folders << path_dataset << actions (act) << "/list_folders.txt";
    field <std::string> list_folders;
    list_folders.load( ss_folders.str() );
    int n_folders = list_folders.n_rows;
    
    for (int i=0; i< n_folders; ++i)
    {
      if (!(actions(act)=="Run-Side" && list_folders(i)=="001"))
      {
	std::ostringstream ss_action;
	ss_action << act;
	
	action_seq_names(k,0) = actions(act); //Action
	action_seq_names(k,1) = list_folders(i); //Video Sequence
	action_seq_names(k,2) = ss_action.str();; //Action index
	k++;
      }
    }
  }
  
  
  std::stringstream load_folder;
  
  load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
  
 
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    field<mat> field_all_actions(n_actions);

      for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
      {
	std::string action_name = action_seq_names(train_i,0);   
	std::string folder_n    = action_seq_names(train_i,1);
	int index_act  = atoi( action_seq_names(test_i,2).c_str() );
	
	if (test_i!=train_i)
	{
	  cout << action_name << "_" << folder_n << " ";
	  std::stringstream load_feat_video_i;
	  load_feat_video_i   << load_folder.str() << "/"     << action_name << "_" << folder_n << "_dim" << dim  << ".h5";

	  mat mat_features_video_i;
	  mat_features_video_i.load( load_feat_video_i.str() , hdf5_binary );
	  
	  field_all_actions(index_act)  =join_rows( field_all_actions(index_act), mat_features_video_i);	  
	  
	}
	
      }
      
      
      

    getchar();
  }
  
/*  
  //borra lo de abajo
  
  std::string action_name = action_seq_names(test_i,0);   
  std::string folder_n    = action_seq_names(test_i,1);
  int act  = atoi( action_seq_names(test_i,2).c_str() );
  
  
  {
    
    for (int act=0; act<n_actions; ++act)
    {
      std::stringstream load_feat_video_i;
      
      
      load_feat_video_i   << load_folder.str() << "/"     << action_name  << "_" << folder_n << "_dim" << dim  << ".h5";
      
    }
    
    
  }
  
}*/


}