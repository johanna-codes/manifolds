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



inline
void
train_kth(field<string> all_people, field<std::string> actions, int N_cent, int scale_factor, int shift, int dim, int sc);

inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int pe_ts);

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
  
  int N_cent = 256; // as per Improved Trajectories Features
  
  field<string> all_people;
  all_people.load(peopleList);
  
  field<std::string> actions;
  actions.load( actionNames );  
  
  train_kth(all_people,  actions, N_cent, scale_factor, shift, dim, sc);
  
  return 0;
}

inline
void
train_kth(field<string> all_people, field<std::string> actions, int N_cent, int scale_factor, int shift, int dim, int sc)
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;

  for (int pe_ts = 0; pe_ts< n_peo; ++pe_ts)
  {
    for (int act=0; act<n_actions; ++act)
    { 
      mat features_action_i;
      for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
      {
	if (pe_tr!= pe_ts) 
	{
	  mat mat_features_video_i;  
	  std::stringstream load_folder;
	  std::stringstream load_feat_video_i;	  
	  load_folder << path << "dim_" << dim <<"/features/kth-features_dim" << dim <<  "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
	  load_feat_video_i << load_folder.str() << "/" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	  mat_features_video_i.load( load_feat_video_i.str(), hdf5_binary );
	  
	  if ( mat_features_video_i.n_cols>0 )
	  {
	    features_action_i	 = join_rows( features_action_i, mat_features_video_i );
	    
	  }
	  else 
	  {
	    cout << "# vectors = 0 in " << load_feat_video_i.str() << endl;     
	  }
	  
	}
	
      }
      
      //ACA voy a tener todos los vectores de la accion _i. GET GMM
      get_gmm(features_action_i, N_cent, dim,  pe_ts);
      
    }
  }

}

// **************************GMM*******************************

inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int pe_ts)
{

   bool is_finite = features_action_i.is_finite();
 
    if (!is_finite )
    {
      cout << "is_finite?? " << is_finite << endl;
      cout << features_action_i.n_rows << " " << features_action_i.n_cols << endl;
      getchar();
    }
  
  
  cout << "universal GMM" << endl;
  gmm_diag gmm_model;
  gmm_diag bg_model;
  
  
  
  bool status_em = false;
  int rep_em=0;
  
  
  int km_iter = 10;
  int em_iter  = 5;
  double var_floor = 1e-10;
  bool print_mode = true;
  
  
  
  
  while (!status_em)
  {
    bool status_kmeans = false;
    //int rep_km = 0;
    
    while (!status_kmeans)
    {
      arma_rng::set_seed_random();
      
      status_kmeans = gmm_model.learn(features_action_i, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(features_action_i, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./GMM_models/run" << pe_ts << "_GMM_Ng" << N_cent << "_dim" <<dim << "_sc1"; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
  
}
