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
const std::string path  = "/home/johanna/codes/codes-git/manifolds/trunk/kth/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///KTH  
field<string> all_people;

field<std::string> actions;


inline
void
train_kth(int N_cent,  int dim, int sc);

inline
float
test_kth(int N_cent, int dim, int sc, float scale_factor, int shift, bool flag_shift );


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
  
  
  /// Train the model. Run once
  //train_kth( N_cent, dim, sc);
  
  /// Test - Shifts
  vec vec_shift;  
  vec all_acc_hor;
  vec all_acc_ver;
  
  vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
  
  all_acc_hor.zeros(vec_shift.n_elem);
  all_acc_ver.zeros(vec_shift.n_elem);
  
  
  bool flag_shift;
  for (int i=0; i<vec_shift.n_elem; ++i)
  {
    int scale_factor =1;
    int shift = vec_shift(i);
    cout << "Doing Horizontal Shift " <<  shift << endl;
    flag_shift = true; 
    all_acc_hor(i) = test_kth( N_cent, dim, sc, scale_factor, shift, flag_shift );
    
    cout << "Doing Vertical Shift " <<  shift << endl;
    flag_shift = false; 
    all_acc_ver(i) = test_kth( N_cent, dim, sc, scale_factor, shift, flag_shift );
  }
  
  vec_shift.t().print();
  all_acc_hor.t().print("Horizontal");
  all_acc_ver.t().print("Vertical");
  
  
  
  
  //   vec scale_vec;
  //   scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.0 << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  //   int shift = 0;
  //   
  //   vec all_acc;
  //   all_acc.zeros(scale_vec.n_elem);
  //   
  //   
  //   for (int i=0; i<scale_vec.n_elem; ++i)
  //   {
  //     
  //     float scale_factor = scale_vec(i);
  //     cout << "scale_factor="  << scale_factor << endl;
  //     all_acc(i) = test_kth( N_cent, dim, sc, scale_factor, shift );
  //   }
  //   
  //   scale_vec.t().print();
  //   all_acc.t().print();
  
  return 0;
}


//*************************************************************
//***************************Train******************************

inline
void
train_kth(int N_cent, int dim, int sc)
{
  all_people.load(peopleList);
  actions.load( actionNames ); 
  
  int scale_factor =1;
  int shift = 0;
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
      
      //GET GMM for each action and save
      get_gmm(features_action_i, N_cent, dim,  pe_ts, act );
      
    }
  }
  
}


//*************************************************************
//***************************Test******************************
inline
float
test_kth(int N_cent, int dim, int sc, float scale_factor, int shift, bool flag_shift )
{
  all_people.load(peopleList);
  actions.load( actionNames ); 
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int n_test = n_peo*n_actions;
  
  vec real_labels;
  vec est_labels;
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  float acc;
  acc = 0;
  int k = 0;
  
  
  for (int pe_ts = 0; pe_ts< n_peo; ++pe_ts)
  {
    
    for (int act=0; act<n_actions; ++act)
    { 
      mat mat_features_video_i;  
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      
      if (flag_shift = true)
      {
	load_folder << path << "dim_" << dim <<"/features/kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-horshift"<< shift ;
      }
      
      if (flag_shift = true)
      {
	load_folder << path << "dim_" << dim <<"/features/kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-vershift"<< shift ;
      }
      
      
      
      load_feat_video_i << load_folder.str() << "/" << all_people (pe_ts) << "_" << actions(act) << "_dim" << dim  << ".h5";
      mat_features_video_i.load( load_feat_video_i.str( ), hdf5_binary );
      
      vec likelihood_actions =  get_loglikelihoods( mat_features_video_i, pe_ts, N_cent, dim);
      
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
    
  }
  
  //cout << "Performance for shift: " << shift << " = " << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
  
}


// **************************GMM*******************************

inline 
void
get_gmm (mat& features_action_i, int N_cent, int dim, int pe_ts, int act )
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
  tmp_ss5 << "./GMM_models/run" << pe_ts << "_" << actions(act) <<  "_GMM_Ng" << N_cent << "_dim" <<dim << "_sc1" ; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
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
    tmp_ss5 << "./GMM_models/run" << run << "_" << actions(act_tr) <<  "_GMM_Ng" << N_cent << "_dim" <<dim << "_sc1" ; 
    //cout << "Loading GMM in " << tmp_ss5.str() << endl;
    gmm_model.load( tmp_ss5.str() );
    
    
    likelihood_actions (act_tr) = gmm_model.avg_log_p(mat_features);
    
  }
  
  
  return likelihood_actions;
  
}
