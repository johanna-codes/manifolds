inline
cov_mat::cov_mat( const std::string in_path,
			  const std::string in_actionNames,  
			  const int in_scale_factor, 
			  const int in_shift,
			  const int in_dim
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim)
{
  actions.load( actionNames );  
}

///****************************************************************************************
///****************************************************************************************
//One covariance matrix per video

inline
void
cov_mat::calculate_one_per_video( )
{
   int n_actions = actions.n_rows;
  
  field <std::string> parallel_names (150,4);  ///From the sataset description
  int k =0;
  

    for (int act=0; act<n_actions; ++act)
    {
      
      std::stringstream  ss_folders;
      ss_folders << path << actions (act) << "/list_folders.txt";
      
      field <std::string> list_folders;
      list_folders.load( ss_folders.str() );
      int n_folders = list_folders.n_rows;
      
      
      for (int i=0; i< n_folders; ++i)
      {
      
      std::stringstream ss_frames_folder;
      ss_frames_folder << path << actions(act) << "/"  << list_folders(i);
      
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      std::stringstream load_labels_video_i;
      
      load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
      
      load_feat_video_i   << load_folder.str() << "/"     << actions(act)  << "_" << list_folders(i) << "_dim" << dim  << ".h5";
      load_labels_video_i << load_folder.str() << "/lab_" << actions(act)  << "_" << list_folders(i) << "_dim" << dim  << ".h5";
      
      parallel_names(k,0) = load_feat_video_i.str();
      parallel_names(k,1) = load_labels_video_i.str();
      parallel_names(k,2) = list_folders(i);
      parallel_names(k,3) = actions(act);
      k++;
      cout << parallel_names(k,0) << endl;
      cout << parallel_names(k,1) << endl;
      cout << parallel_names(k,2) << endl;
      cout << parallel_names(k,3) << endl;
    }
  }
   

//   #pragma omp parallel for 
//     for (int k = 0; k< parallel_names.n_rows; ++k)
//     {
//       std::string load_feat_video_i   = parallel_names(k,0);
//       std::string load_labels_video_i = parallel_names(k,1);
//       std::string folder_n = parallel_names(k,2);
//       std::string action_name= parallel_names(k,3);
//       
//       one_video_one_cov(load_feat_video_i, load_labels_video_i, folder_n, action_name );
//     }

}


inline
void
cov_mat::one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i, std::string folder_n, std::string action_name )
{

  mat mat_features_video_i;
  
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );

  int n_vec = mat_features_video_i.n_cols;

  std::stringstream save_folder;
  save_folder << "./one-cov-mat/scale" << scale_factor << "-shift"<< shift ;
  
  running_stat_vec<rowvec> stat_seg(true);
  
  for (int l=0; l<n_vec; ++l)
  {
    
    vec sample = mat_features_video_i.col(l);
    stat_seg (sample);
    
  }


    std::stringstream save_cov_seg;
    save_cov_seg << save_folder.str() << "/cov_" << action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
    
    std::stringstream save_LogMcov_seg;
    save_LogMcov_seg << save_folder.str() << "/LogMcov_"  << action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
      
    double THRESH = 0.000001;
    mat cov_seg_i = stat_seg.cov();
    
    //Following Mehrtash suggestions as per email dated June26th 2014
    cov_seg_i = 0.5*(cov_seg_i + cov_seg_i.t());
    vec D;
    mat V;
    eig_sym(D, V, cov_seg_i);
    uvec q1 = find(D < THRESH);
    
    if (q1.n_elem>0)
    {
      for (uword pos = 0; pos < q1.n_elem; ++pos)
      {
	D( q1(pos) ) = THRESH;
      }
      cov_seg_i = V*diagmat(D)*V.t();  
      
    }  
    //end suggestion

     eig_sym(D, V, cov_seg_i);
     mat log_M = V*diagmat( log(D) )*V.t();
     
     #pragma omp critical
     {
     cov_seg_i.save( save_cov_seg.str(), hdf5_binary ); 
     log_M.save( save_LogMcov_seg.str(), hdf5_binary );
     }

      

  }




