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
train_ucf(field <std::string> action_seq_names, int N_cent,  int dim, int sc);

inline
float
test_ucf(field <std::string> action_seq_names, int N_cent, int dim, int sc, int scale_factor, int shift );


inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int test_i, int act );

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
  
  int k=0;
  field <std::string> action_seq_names (150-1,3);  ///From the dataset description (150)
  int n_actions = actions.n_rows;

  
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
  
  
  //**********Train the mode**********************
  train_ucf( action_seq_names, N_cent, dim, sc);
  
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


//***********************************************************************
//*******************************Train***********************************

inline
void
train_ucf(field <std::string> action_seq_names, int N_cent, int dim, int sc)
{
  actions.load( actionNames ); 
  
  int scale_factor =1;
  int shift = 0;
  int n_actions = actions.n_rows;
  
  std::stringstream load_folder;
  load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
  
 
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {

    cout << "Doing for " << test_i << endl;
    
    field<mat> field_all_actions(n_actions);
    
    for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
    {
	std::string action_name = action_seq_names(train_i,0);   
	std::string folder_n    = action_seq_names(train_i,1);
	int index_act  = atoi( action_seq_names(train_i,2).c_str() );
	
	if (test_i!=train_i)
	{
	  //cout << action_name << "_" << folder_n << " ";
	  std::stringstream load_feat_video_i;
	  load_feat_video_i   << load_folder.str() << "/"     << action_name << "_" << folder_n << "_dim" << dim  << ".h5";

	  mat mat_features_video_i;
	  mat_features_video_i.load( load_feat_video_i.str() , hdf5_binary );
	  
	  field_all_actions(index_act)  =join_rows( field_all_actions(index_act), mat_features_video_i);	  
	}
      }
      
      for (int act=0; act<n_actions; ++act)
      {
	mat features_action_i;
	features_action_i = field_all_actions(act);
	field_all_actions(act).reset();
	cout << "Final r&c "<<  features_action_i.n_rows << " & " << features_action_i.n_cols << endl;
	get_gmm (features_action_i, N_cent,  dim,  test_i,  act );
      }

  }
  
}

// ************************************GMM********************************************
// ***********************************************************************************

inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int test_i, int act )
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
  tmp_ss5 << "./GMM_models/run" << test_i << "_" << actions(act) <<  "_GMM_Ng" << N_cent << "_dim" <<dim; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
}


inline
float
test_ucf(field <std::string> action_seq_names, int N_cent, int dim, int sc, int scale_factor, int shift )
{
  actions.load( actionNames ); 
  
  int n_actions = actions.n_rows;

  
  int n_test = action_seq_names.n_rows;
  
  vec real_labels;
  vec est_labels;
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  float acc;
  acc = 0;
  int k = 0;
  
  std::stringstream load_folder;
  load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
  
  
  
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    std::string action_name = action_seq_names(test_i,0);   
    std::string folder_n    = action_seq_names(test_i,1);
    int act  = atoi( action_seq_names(test_i,2).c_str() );
    
     std::stringstream load_feat_video_i;
     load_feat_video_i   << load_folder.str() << "/"     << action_name << "_" << folder_n << "_dim" << dim  << ".h5";
     
     mat mat_features_video_i;
     mat_features_video_i.load( load_feat_video_i.str() , hdf5_binary );
    
      
      vec likelihood_actions =  get_loglikelihoods( mat_features_video_i, test_i, N_cent, dim);
      
      uword est_label_video_i;
      likelihood_actions.max(est_label_video_i);
      
      real_labels(k)= act;
      est_labels(k) = est_label_video_i;
      ++k;
      
      if (est_label_video_i == act)
      {
	acc++;  
      }
      //cout << "Real label is " <<  act << " and it was classified as " << est_label_video_i << endl;


  }
  
  cout << "Performance for shift: " << shift << " = " << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
  
}

inline
vec
get_loglikelihoods(mat &mat_features, int run, int N_cent, int dim)
{
  vec likelihood_actions(actions.n_rows);
  
  for (uword act_tr=0; act_tr < actions.n_rows; ++act_tr)
  {
    gmm_diag gmm_model;
    
    std::stringstream tmp_ss5;
    tmp_ss5 << "./GMM_models/run" << test_i << "_" << actions(act) <<  "_GMM_Ng" << N_cent << "_dim" <<dim;    //cout << "Loading GMM in " << tmp_ss5.str() << endl;
    gmm_model.load( tmp_ss5.str() );
    
    likelihood_actions (act_tr) = gmm_model.avg_log_p(mat_features);
    
  }

  return likelihood_actions;
  
}


