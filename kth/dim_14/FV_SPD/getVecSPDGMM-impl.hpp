inline
getVecSPD_GMM::getVecSPD_GMM( const std::string in_path,
			      const std::string in_actionNames,  
			      const float in_scale_factor, 
			      const int in_shift,
			      const int in_scene, //only for kth
			      const int in_dim
			      
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), dim(in_dim)
{
  actions.load( actionNames );  
}



///****************************************************************************************
///****************************************************************************************
//One covariance matrix per video

inline
void
getVecSPD_GMM::get_one_vec_video( field<string> in_all_people)
{
  all_people = in_all_people;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int sc = total_scenes; //Solo estoy usando 1 
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      
      
      std::ostringstream load_folder;
      load_folder << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      
      std::stringstream load_Covs;
      load_Covs << load_folder.str() << "/Cov_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
      
      mat cov_i;
      cov_i.load( load_Covs.str(), hdf5_binary ); 
      //cov_i.print("Cov1");
      
      int dim_spdvec  = dim*( dim + 1 )/2;
      
      vec vecSPD_cov ;
      vecSPD_cov = get_vec (cov_i, dim_spdvec);
      
      
      std::ostringstream save_spd_vec;
      save_spd_vec << "./vec_spd/sc" << sc << "/scale" << scale_factor << "-shift"<< shift << "/vecSPD_" <<all_people (pe) << "_" << actions(act) <<  ".h5";
      
      vecSPD_cov.save(save_spd_vec.str(), hdf5_binary);
      

//       mat mycov;      
//       mycov << 9  << 13 << 3 << 6 << endr
//       << 13 << 11 << 7 << 6 << endr
//       << 3  << 7  << 4 << 7 << endr
//       << 6  << 6  << 7 << 10 << endr;
//       mycov.print();
//	mat U = trimatu( mycov );
//	getchar();
      
    }
  }
  
  
}


inline
void
getVecSPD_GMM::get_GMM( field<string> in_all_people, int N_cent)
{
  all_people = in_all_people;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int sc = total_scenes; //Solo estoy usando 1 
  
  mat all_vectors;
  int dim_spdvec  = dim*( dim + 1 )/2;
  
  all_vectors.zeros(dim_spdvec,n_peo*n_actions );
  int k=0;
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      
      
      
      
      std::ostringstream load_spd_vec;
      load_spd_vec << "./vec_spd/sc" << sc << "/scale" << scale_factor << "-shift"<< shift << "/vecSPD_" <<all_people (pe) << "_" << actions(act) <<  ".h5";
      
      vec vecSPD_cov;
      vecSPD_cov.load(load_spd_vec.str());
      all_vectors.col(k)=vecSPD_cov;
      
      k++;
      
      
    }
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
      
      status_kmeans = gmm_model.learn(all_vectors, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(all_vectors, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./universal_GMM/UniversalGMM_Ng" << N_cent; 
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
  ss_weigths << "./universal_GMM/weights_Ng" << N_cent <<  ".dat"; 
  
  std::stringstream ss_means;
  ss_means << "./universal_GMM/means_Ng" << N_cent << ".dat"; 
  
  std::stringstream ss_covs;
  ss_covs << "./universal_GMM/covs_Ng" << N_cent << ".dat"; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii);
  

}



///****************************************************************************************
///****************************************************************************************
//More than one covariance matrix per video

inline
void
getVecSPD_GMM::get_vecs_video( field<string> in_all_people, int num_SPD)
{
  all_people = in_all_people;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int sc = total_scenes; //Solo estoy usando 1 
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
     
      for (int k=1; k<=num_SPD; ++k)
      {
	
      
      
      std::ostringstream load_folder;
      load_folder << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      
      std::stringstream load_Covs;
      load_Covs << load_folder.str() << "/Cov_"<< k << "_out_" << num_SPD << "_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
      
      mat cov_i;
      cov_i.load( load_Covs.str(), hdf5_binary ); 
      //cov_i.print("Cov1");
      
      int dim_spdvec  = dim*( dim + 1 )/2;
      
      vec vecSPD_cov ;
      vecSPD_cov = get_vec (cov_i, dim_spdvec);
      
      
      std::ostringstream save_spd_vec;
      save_spd_vec << "./vec_spd/sc" << sc << "/scale" << scale_factor << "-shift"<< shift << "/vecSPD_" << k << "_out_" << num_SPD << "_" <<all_people (pe) << "_" << actions(act) <<  ".h5";
      
      vecSPD_cov.save(save_spd_vec.str(), hdf5_binary);
      

//       mat mycov;      
//       mycov << 9  << 13 << 3 << 6 << endr
//       << 13 << 11 << 7 << 6 << endr
//       << 3  << 7  << 4 << 7 << endr
//       << 6  << 6  << 7 << 10 << endr;
//       mycov.print();
//	mat U = trimatu( mycov );
//	getchar();
    }
      
    }
  }
  
  
}

inline
void
getVecSPD_GMM::
get_GMM_all_vecs( field<string> in_all_people, int N_cent, int num_SPD )
{
  
 all_people = in_all_people;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int sc = total_scenes; //Solo estoy usando 1 
  
  mat all_vectors;
  int dim_spdvec  = dim*( dim + 1 )/2;
  
  all_vectors.zeros(dim_spdvec,n_peo*n_actions*num_SPD );
  int k=0;
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      for (int l=1; l<=num_SPD; ++l)
      {
	
	std::ostringstream load_spd_vec;
	load_spd_vec << "./vec_spd/sc" << sc << "/scale" << scale_factor << "-shift"<< shift << "/vecSPD_" << l << "_out_" << num_SPD << "_" <<all_people (pe) << "_" << actions(act) <<  ".h5";
	vec vecSPD_cov;
	vecSPD_cov.load(load_spd_vec.str(), hdf5_binary);
	all_vectors.col(k)=vecSPD_cov;
	k++;
      
      } 
    }
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
      
      status_kmeans = gmm_model.learn(all_vectors, N_cent, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(all_vectors, N_cent, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  
  //cout <<"EM was repeated " << rep_em << endl;
  
  std::stringstream tmp_ss5;
  tmp_ss5 << "./universal_GMM/UniversalGMM_Ng" << N_cent << "_numSPD_" << num_SPD; 
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
  ss_weigths << "./universal_GMM/weights_Ng" << N_cent << "_numSPD_" << num_SPD <<  ".dat"; 
  
  std::stringstream ss_means;
  ss_means << "./universal_GMM/means_Ng" << N_cent << "_numSPD_" << num_SPD <<  ".dat"; 
  
  std::stringstream ss_covs;
  ss_covs << "./universal_GMM/covs_Ng" << N_cent << "_numSPD_" << num_SPD <<  ".dat"; 
  
  weights.save( ss_weigths.str(), raw_ascii );
  means.save( ss_means.str(), raw_ascii );
  covs.save(ss_covs.str(), raw_ascii); 
  
}


/// Vectorise
inline
vec
getVecSPD_GMM::get_vec( mat cov, int dim_spdvec )
{
  
  vec vec_spd;
  vec_spd.zeros(dim_spdvec);
  int k =0;
  float sr2 = sqrt(2);

  
  for (int i=0; i<4; ++i)
  {
    for (int j=i; j<4; ++j)
    {
      if (i==j)
      {
	vec_spd(k) = cov(i,j);	
      }
      else
      {
	vec_spd(k) = sr2*cov(i,j);	
      }
      k++;  
    }
    
  }
  
  //vec_spd.t().print("vec_spd");
  
  return vec_spd;
}