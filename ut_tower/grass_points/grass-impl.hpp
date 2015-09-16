inline
grass_points::grass_points( const std::string in_path,
			    const std::string in_actionNames,  
			    const float in_scale_factor, 
			    const int in_shift,
			    const int in_dim,		    
			    const int in_p
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim), p(in_p)
{
  actions.load( actionNames );  
}

// One Grassmann Point per Video

inline
void
grass_points::calculate_onepervideo( )
{
  
  int n_actions = actions.n_rows;
  field <std::string> parallel_names (108,3);  ///From the dataset description
  int k = 0;
  
    for (int actID=0; actID<n_actions; ++actID)
    {
      for (int seqID=0; seqID< 12; ++seqID) ///From the sataset description
      {
      
       
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      
      load_folder << path << "features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
      
      load_feat_video_i   << load_folder.str() << "/" << actions(actID)  << "_seq" <<  seqID  <<".h5";
      
      std::ostringstream ss_seqID;
      ss_seqID << seqID;
      
      parallel_names(k,0) = load_feat_video_i.str();
      parallel_names(k,1) = actions(actID);
      parallel_names(k,2) = ss_seqID.str();
       k++;
    }
  }
   

   //#pragma omp parallel for 
     for (int k = 0; k< parallel_names.n_rows; ++k)
     {
       std::string load_feat_video_i   = parallel_names(k,0);
       std::string action_name= parallel_names(k,1);
       int seqID  = atoi( parallel_names(k,2).c_str() );
       
       //one_video_one_cov(load_feat_video_i, action_name, seqID );
       one_video_one_point(load_feat_video_i,  action_name, seqID );
       
       //getchar();
     }

   
}


inline
void
grass_points::one_video_one_point( std::string load_feat_video_i, std::string action_name, int  seqID)
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
    save_Gnp << save_folder.str() << "/grass_pt_" << action_name << "_seq" <<  seqID << "_p" << p << ".h5";
    cout << save_Gnp.str() << endl;
    
    //#pragma omp critical
    Gnp.save( save_Gnp.str(), hdf5_binary ); 
    
  }
  else
  {
    cout << load_feat_video_i << endl;
    getchar();
  }
  
}



