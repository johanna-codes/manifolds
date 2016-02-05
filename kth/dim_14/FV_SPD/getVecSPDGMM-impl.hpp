inline
getVecSPD_GMM::getVecSPD_GMM( const std::string in_path,
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
getVecSPD_GMM::get_SVD( field<string> in_all_people, int  in_dim  )
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
  
  //#pragma omp parallel for 
  for (int k = 0; k< parallel_names.n_rows; ++k)
  {
    std::string load_feat_video_i   = parallel_names(k,0);
    std::string load_labels_video_i = parallel_names(k,1);
    
    int pe   = atoi( parallel_names(k,2).c_str() );
    int act  = atoi( parallel_names(k,3).c_str() );
    
   
    
    
    load_folder << path << "./covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
    
     std::stringstream load_Covs;
    load_Covs << load_folder.str() << "/Cov_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    mat cov_i;
    cov_1.load( load_Covs.str(), hdf5_binary ); 
    cov_1.print("Cov1");
    getchar();
  }
  
}

