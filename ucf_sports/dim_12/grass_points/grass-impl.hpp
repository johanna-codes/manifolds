inline
grass_points::grass_points( const std::string in_path,
			    const std::string in_path_dataset,
			    const std::string in_actionNames,  
			    const int in_scale_factor, 
			    const int in_shift,
			    const int in_dim,		    
			    const int in_p
)
:path(in_path), path_dataset(in_path_dataset), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim), p(in_p)
{
  actions.load( actionNames );  
}

// One Grassmann Point per Video

inline
void
grass_points::calculate_onepervideo( )
{
  
  int n_actions = actions.n_rows;
  field <std::string> parallel_names (150,3);  ///From the sataset description
  int k = 0;
  
  for (int act=0; act<n_actions; ++act)
  {
    
    std::stringstream  ss_folders;
    ss_folders << path_dataset << actions (act) << "/list_folders.txt";
    
    field <std::string> list_folders;
    list_folders.load( ss_folders.str() );
    int n_folders = list_folders.n_rows;
    //cout << list_folders << endl;
    //list_folders.print();
    //getchar();
    
    for (int i=0; i< n_folders; ++i)
    {
      
      std::stringstream ss_frames_folder;
      ss_frames_folder << path << actions(act) << "/"  << list_folders(i);
      
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      std::stringstream load_labels_video_i;
      
      load_folder << path <<"dim_" << dim <<  "/features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
      
      load_feat_video_i   << load_folder.str() << "/"     << actions(act)  << "_" << list_folders(i) << "_dim" << dim  << ".h5";
      //load_labels_video_i << load_folder.str() << "/lab_" << actions(act)  << "_" << list_folders(i) << "_dim" << dim  << ".h5";
      
      parallel_names(k,0) = load_feat_video_i.str();
      //parallel_names(k,1) = load_labels_video_i.str();
      parallel_names(k,1) = list_folders(i);
      parallel_names(k,2) = actions(act);
      
      //       cout << parallel_names(k,0) << endl;
      //       cout << parallel_names(k,1) << endl;
      //       cout << parallel_names(k,2) << endl;
      //       cout << parallel_names(k,3) << endl;
      //       getchar();
      k++;
    }
  }
  
  
  
  
  
  //omp_set_num_threads(4); //Use only 8 processors
  
  //#pragma omp parallel for 
  for (int k = 0; k< parallel_names.n_rows; ++k)
  {
    
    std::string load_feat_video_i   = parallel_names(k,0);
    //std::string load_labels_video_i = parallel_names(k,1);
    std::string folder_n = parallel_names(k,1);
    std::string action_name= parallel_names(k,2);
    
    
    
    //#pragma omp critical
    //cout <<  all_people (pe) << "_" << actions(act) << endl;
    
    one_video_one_point(load_feat_video_i,  folder_n, action_name );
  }
  
}


inline
void
grass_points::one_video_one_point( std::string load_feat_video_i, std::string folder_n, std::string action_name )
{
  //cout << load_feat_video_i << endl;
  mat mat_features_video_i;
  
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );
  
  
  //Hay problemas con:
  ///Run-Side_001_dim14 
  // mat size: 14 & 7
  //
  if (mat_features_video_i.n_cols > 100)
  {  
    
    std::stringstream save_folder;
    save_folder << "./one-grass-point/scale" << scale_factor << "-shift"<< shift ;
    
    
    mat U; vec s;   mat V;
    svd_econ(U,s,V,mat_features_video_i); 
    mat Gnp = U.cols(0,p-1);
    
    std::stringstream save_Gnp;
    //cout << save_folder.str() << endl;
    save_Gnp << save_folder.str() << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
    cout << save_Gnp.str() << endl;
    
    //#pragma omp critical
    Gnp.save( save_Gnp.str(), hdf5_binary ); 
    
  }
  else
  {
    cout << load_feat_video_i << endl;
    //getchar();
  }
  
}



