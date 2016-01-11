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
      
      
  running_stat_vec<rowvec> stat_seg(true);
  
  for (int l=0; l<n_vec; ++l)
  {
    
    vec sample = mat_features_video_i.col(l);
    stat_seg (sample);
    
  }


    std::stringstream save_CovMeans;
    save_CovMeans << save_folder.str() << "/CovMeans_" <<  all_people (pe) << "_" << actions(act) <<  ".h5";
    
    std::stringstream save_LogCovMeans;
    save_LogCovMeans << save_folder.str() << "/LogM_CovMeans_" << all_people (pe) << "_" << actions(act) << ".h5";
      
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
    //end suggestion
    
    
    
    /// Taken from A Novel Earth Mover’s Distance Methodology for Image Matching with Gaussian Mixture Models
    // Eq. (4)
    
    mat CovMean = zeros(dim+1,dim+1);
    
    //CovMean.print();
    //cout << cov_i.n_rows<< " & " << cov_i.n_cols << endl;
    //cout << mean_i.n_rows<< " & " << mean_i.n_cols << endl;
    
    
    
    CovMean.submat( 0, 0, dim-1, dim-1) = cov_i;    
    CovMean.submat( 0, dim, dim-1, dim )  = mean_i;
    CovMean.submat(dim,0,dim,dim-1) = mean_i.t();
    CovMean(dim,dim) = 1;
    
    
    
    // end

     eig_sym(D, V, CovMean);
     mat log_M = V*diagmat( log(D) )*V.t();
     
     #pragma omp critical
     {
     cout << "saving " <<  all_people (pe) << endl;
     CovMean.print();
     log_M.print();
     cout << endl;
     log(D).print();
     
     V.print();
     cout << endl;
     D.print();
     getchar();
     CovMean.save( save_CovMeans.str(), hdf5_binary ); 
     log_M.save( save_LogCovMeans.str(), hdf5_binary );
     }

      

  }




