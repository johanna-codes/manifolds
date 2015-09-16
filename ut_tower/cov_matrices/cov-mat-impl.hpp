inline
cov_mat::cov_mat( const std::string in_path,
			  const std::string in_actionNames,  
			  const float in_scale_factor, 
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
  
  field <std::string> parallel_names (108,3);  ///From the sataset description
  int k = 0;
  

    for (int actID=0; actID<n_actions; ++actID)
    {
      
           
      for (int seqID=0; seqID< 12; ++seqID) ///From the sataset description
      {
      
       
      std::stringstream load_folder;
      std::stringstream load_feat_video_i;
      
      load_folder << path << "features/features_dim" << dim << "/scale" << scale_factor << "-shift"<< shift;
      
      load_feat_video_i   << load_folder.str() << "/" << actions(actID)  << "_seq" <<  seqID << ".h5";
      
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
       
       one_video_one_cov(load_feat_video_i, action_name, seqID );
       
       //getchar();
     }

}


inline
void
cov_mat::one_video_one_cov( std::string load_feat_video_i, std::string action_name, int seqID )
{

  mat mat_features_video_i;
  cout << "Doing for " << load_feat_video_i << endl;
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
    save_cov_seg << save_folder.str() << "/cov_" << action_name << "_seq" <<  seqID << ".h5";
    
    std::stringstream save_LogMcov_seg;
    save_LogMcov_seg << save_folder.str() << "/LogMcov_"  << action_name << "_seq" <<  seqID << ".h5";
      
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




