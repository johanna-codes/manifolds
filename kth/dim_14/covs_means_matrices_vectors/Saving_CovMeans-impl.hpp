inline
CovMeans_mat_kth::CovMeans_mat_kth( const std::string in_path,
				    const std::string in_actionNames,  
				    const float in_scale_factor, 
				    const int in_shift,
				    const int in_scene //only for kth
				    
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene)
{
  actions.load( actionNames );  
}



///****************************************************************************************
///****************************************************************************************
//One covariance matrix per video

inline
void
CovMeans_mat_kth::calculate_one_per_video( field<string> in_all_people, int  in_dim  )
{
  all_people = in_all_people;
  dim = in_dim;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  
  field <std::string> parallel_names(n_peo*n_actions,4); 
  int sc = total_scenes; //Solo estoy usando 1 
  int k =0;
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      
      
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      std::stringstream load_labels_video_i;
      
      // Shifting both
      load_folder << path <<"kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      //load_folder << path <<"kth-features_dim" << dim <<  "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      
      
      //      If you want to use. You have to add the  flag_shift in this method. 
      //	if (flag_shift) //Horizontal Shift
      //       {
	// 	load_folder << path << "/kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-horshift"<< shift ;
      //       
      //       }
      //       
      //       if (!flag_shift)//Vertical Shift
      //       {
	// 	load_folder << path << "./kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-vershift"<< shift ;
      // 	
      // 	
      //       }
      //       
      
      
      
      load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      
      
      std::ostringstream ss1;
      std::ostringstream ss2;
      ss1 << pe;
      ss2 << act;
      
      
      parallel_names(k,0) = load_feat_video_i.str();
      parallel_names(k,1) = load_labels_video_i.str();
      parallel_names(k,2) = ss1.str();
      parallel_names(k,3) = ss2.str();
      k++;
      
    }
  }
  
  
  omp_set_num_threads(1); //Use only 8 processors
  
  #pragma omp parallel for 
  for (int k = 0; k< parallel_names.n_rows; ++k)
  {
    std::string load_feat_video_i   = parallel_names(k,0);
    std::string load_labels_video_i = parallel_names(k,1);
    
    int pe   = atoi( parallel_names(k,2).c_str() );
    int act  = atoi( parallel_names(k,3).c_str() );
    
    one_video_one_cov(load_feat_video_i, load_labels_video_i, sc, pe, act );
    //getchar();
  }
  
}


inline
void
CovMeans_mat_kth::one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act )
{
  //   #pragma omp critical
  //   {
    //   cout << load_feat_video_i << endl;
    //   getchar();
    //   }
    
    mat mat_features_video_i;
    
    mat_features_video_i.load( load_feat_video_i, hdf5_binary );
    
    int n_vec = mat_features_video_i.n_cols;
    
    
    std::stringstream save_folder;
    
    //Shifting both
    save_folder << "./CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
    
    {
      //      If you want to use. You have to add the  flag_shift in this method. 
      //	   if (flag_shift) //Horizontal Shift
      //       {
	// 
	// 	save_folder << "./kth-one-CovsMeans-mat/sc" << sc << "/scale" << scale_factor <<  "-horshift"<< shift ;
	// 	
	//       }
	//       
	//       if (!flag_shift)//Vertical Shift
	//       {
	  // 	 save_folder << "./kth-one-CovsMeans-mat/sc" << sc << "/scale" << scale_factor << "-vershift"<< shift ;
	  //       }
	  //       
    }   
    
    running_stat_vec<rowvec> stat_seg(true);
    
    for (int l=0; l<n_vec; ++l)
    {
      
      vec sample = mat_features_video_i.col(l);
      stat_seg (sample);
      
    }
    
    
    std::stringstream save_Covs;
    save_Covs << save_folder.str() << "/Cov_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    std::stringstream save_logMCovs;
    save_logMCovs << save_folder.str() << "/logM_Cov_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    
    std::stringstream save_Means;
    save_Means << save_folder.str() << "/Means_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    
    
    double THRESH = 0.000001;
    mat cov_i = stat_seg.cov();
    vec mean_i = stat_seg.mean();
    
    
    
    //Following Mehrtash suggestions as per email dated June26th 2014
    cov_i = 0.5*(cov_i + cov_i.t());
    vec D;
    mat V;
    eig_sym(D, V, cov_i);
    uvec q1 = find(D < THRESH);
    
    if (q1.n_elem>0)
    {
      for (uword pos = 0; pos < q1.n_elem; ++pos)
      {
	D( q1(pos) ) = THRESH;
      }
      cov_i = V*diagmat(D)*V.t();  
      
    }  
    //end suggestions
    
    
    eig_sym(D, V, cov_i);
    mat logM_cov_i = V*diagmat( log(D) )*V.t();
    
    
    
    #pragma omp critical
    {
      cout << "saving " <<  all_people (pe) << endl;
      
      cov_i.save(  save_Covs.str(), hdf5_binary ); 
      logM_cov_i.save( save_logMCovs.str(), hdf5_binary ); 
      mean_i.save( save_Means.str(), hdf5_binary ); 
    }
    
    
    
}


///****************************************************************************************
///******************************Using GMM*************************************************


inline
void
CovMeans_mat_kth::get_gmm_per_video( field<string> in_all_people, int  in_dim, const int Ng  )
{
  all_people = in_all_people;
  dim = in_dim;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  
  field <std::string> parallel_names(n_peo*n_actions,4); 
  int sc = total_scenes; //Solo estoy usando 1 
  int k =0;
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      
      
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      std::stringstream load_labels_video_i;
      
      // Shifting both
      load_folder << path <<"kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      
      std::ostringstream ss1;
      std::ostringstream ss2;
      ss1 << pe;
      ss2 << act;
      
      
      parallel_names(k,0) = load_feat_video_i.str();
      parallel_names(k,1) = load_labels_video_i.str();
      parallel_names(k,2) = ss1.str();
      parallel_names(k,3) = ss2.str();
      k++;
      
    }
  }
  
  
  //omp_set_num_threads(1); //Use only 8 processors
  
  // It can't be done in parallel because GMM runs in parallel
  //#pragma omp parallel for. //
  for (int k = 0; k< parallel_names.n_rows; ++k)
  {
    std::string load_feat_video_i   = parallel_names(k,0);
    std::string load_labels_video_i = parallel_names(k,1);
    
    int pe   = atoi( parallel_names(k,2).c_str() );
    int act  = atoi( parallel_names(k,3).c_str() );
    
    gmm_one_video(load_feat_video_i, load_labels_video_i, sc, pe, act, Ng );
    //getchar();
  }
  
}


inline
void
CovMeans_mat_kth::gmm_one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act, const int Ng )
{
  
  cout <<  all_people (pe) << "_" << actions(act)  << endl;
  mat mat_features_video_i;
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );
  int n_vec = mat_features_video_i.n_cols;
  
  
  bool is_finite = mat_features_video_i.is_finite();
  
  if (!is_finite )
  {
    cout << "is_finite?? " << is_finite << endl;
    cout << mat_features_video_i.n_rows << " " << mat_features_video_i.n_cols << endl;
    getchar();
  }
  
  
  gmm_diag gmm_model;
  gmm_diag bg_model;
  
  
  bool status_em = false;
  int rep_em=0;
  
  int km_iter = 10;
  int em_iter  = 5;
  double var_floor = 1e-10;
  bool print_mode = false;
  
  
  while (!status_em)
  {
    bool status_kmeans = false;
    //int rep_km = 0;
    
    while (!status_kmeans)
    {
      arma_rng::set_seed_random();
      
      status_kmeans = gmm_model.learn(mat_features_video_i, Ng, eucl_dist, random_subset, km_iter, 0, var_floor, print_mode);   //Only Kmeans
      bg_model = gmm_model;
      //rep_km++;
    }
    
    
    status_em = gmm_model.learn(mat_features_video_i, Ng, eucl_dist, keep_existing, 0, em_iter, var_floor, print_mode);   
    rep_em++;
    
    if (rep_em==9)
    {
      status_em = true;
      gmm_model = bg_model;
      
    }
    
  }
  
  cout <<"EM was repeated " << rep_em << endl << endl;
  
  mat dcov = gmm_model.dcovs;
  mat means = gmm_model.means; 
 
  mat cov_i, logM_cov_i, V; 
  vec mean_i, D;
  
  std::stringstream save_folder;
  save_folder << "./CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
  for (int i=1; i<=Ng; ++i)
  {
     
    //cout << i << " out " << Ng << endl;
    cov_i = diagmat( dcov.col(i-1) );
    cov_i = mehrtash_suggestion(cov_i);
    
    mean_i = means.col(i -1);
    
    eig_sym(D, V, cov_i);
    mat logM_cov_i = V*diagmat( log(D) )*V.t();
    
    
    std::stringstream save_Covs;
    save_Covs << save_folder.str() << "/Cov_"<< i << "out" << Ng << "_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    std::stringstream save_logMCovs;
    save_logMCovs << save_folder.str() << "/logM_Cov_" << i << "out" << Ng << "_"  <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    std::stringstream save_Means;
    save_Means << save_folder.str() << "/Means_" << i << "out" << Ng << "_"  <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    cov_i.save(  save_Covs.str(), hdf5_binary ); 
    logM_cov_i.save( save_logMCovs.str(), hdf5_binary ); 
    mean_i.save( save_Means.str(), hdf5_binary ); 

    
  }
  
}




inline
mat
CovMeans_mat_kth::mehrtash_suggestion(mat cov_i)
{
  //Following Mehrtash suggestions as per email dated June26th 2014
  mat new_covi;
  
  double THRESH = 0.000001;
  new_covi = 0.5*(cov_i + cov_i.t());
  vec D;
  mat V;
  eig_sym(D, V, new_covi);
  uvec q1 = find(D < THRESH);
  
  
  if (q1.n_elem>0)
  {
    for (uword pos = 0; pos < q1.n_elem; ++pos)
    {
      D( q1(pos) ) = THRESH;
      
    }
    
    new_covi = V*diagmat(D)*V.t();  
    
  }  
  
  return new_covi;
    //end suggestions
}
