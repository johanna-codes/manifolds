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

int
main(int argc, char** argv)
{
  
  
  
  int scale_factor =1;
  int shift = 0;
  int dim = 14; 
  int sc =1;
  
  int N_cent = 256; // as per Improved Trajectories Features
  
  
  
  field<std::string> actions;
  actions.load( actionNames );  
  int n_actions = actions.n_rows;
  
  field <std::string> action_seq_names (150,3);  ///From the dataset description (150)
  
  mat uni_features;
   
   
  for (int act=0; act<n_actions; ++act)
  {
    std::stringstream  ss_folders;
    ss_folders << path_dataset << actions (act) << "/list_folders.txt";
    field <std::string> list_folders;
    list_folders.load( ss_folders.str() );
    int n_folders = list_folders.n_rows;
    //cout <<  actions (act) << endl;
    //list_folders.print();
    
    mat mat_features_tmp;
    mat mat_features;
    for (int i=0; i< n_folders; ++i)
    {
      std::stringstream ss_frames_folder;
      ss_frames_folder << path << actions(act) << "/"  << list_folders(i);
      
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      
      load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
      
      load_feat_video_i   << load_folder.str() << "/"     << actions(act)  << "_" << list_folders(i) << "_dim" << dim  << ".h5";
      cout << load_feat_video_i.str() << endl;
      mat mat_features_video_i;
      mat_features_video_i.load( load_feat_video_i.str() , hdf5_binary );
      
      if ( mat_features_video_i.n_cols>0 )
      {
	mat_features_tmp	 = join_rows( mat_features_tmp, mat_features_video_i );
	
      }
      
      else 
      {
	cout << "# vectors = 0 in " << load_feat_video_i.str() << endl;     	
      }
      
    }
    
    const uword N_max = 25600; //25600x10acciones = 256.000 as per Improved Trajectories Features
    
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
    
    //Adding to 
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
  
  // **************************universal GMM*******************************
  
  bool is_finite = uni_features.is_finite();
  
  if (!is_finite )
  {
    cout << "is_finite?? " << is_finite << endl;
    cout << uni_features.n_rows << " " << uni_features.n_cols << endl;
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
      
      status_kmeans = gmm_model.learn(uni_features, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(uni_features, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./universal_GMM/UniversalGMM_Ng" << N_cent << "_dim" <<dim << "_sc1"; 
  cout << "Saving GMM in " << tmp_ss5.str() << endl;
  gmm_model.save( tmp_ss5.str() );
  cout << endl;
  
  mat means;
  mat covs;
  vec weights;	
  
  means = gmm_model.means;
  covs  = gmm_model.dcovs;
  weights = gmm_model.hefts.t();	
  
  //Saving statistics
  std::stringstream ss_weigths;
  ss_weigths << "./universal_GMM/weights_Ng" << N_cent << "_dim" << dim << "_sc1" << ".dat"; 
  
  std::stringstream ss_means;
  ss_means << "./universal_GMM/means_Ng" << N_cent << "_dim" <<dim << "_sc1" << ".dat"; 
  
  std::stringstream ss_covs;
  ss_covs << "./universal_GMM/covs_Ng" << N_cent << "_dim" <<dim << "_sc1" << ".dat"; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii);
  
  
  
  return 0;
  
}
