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
getVecSPD_GMM::get_SVD( field<string> in_all_people)
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
      
      int dim  = cov_i.n_cols*(cov_i.n_cols+1)/2;
      
      mat mycov;
      
      mycov << 9  << 13 << 3 << 6 << endr
	    << 13 << 11 << 7 << 6 << endr
	    << 3  << 7  << 4 << 7 << endr
	    << 6  << 6  << 7 << 10 << endr;


	    
	    mycov.print();
	    mat U = trimatu( mycov );
	    
	    vec vec_spd;
	    vec_spd.zeros(10);
	    int k =0;
	    float sr2 = sqrt(2);
	    U.print("Upper mycov");
	    
	    for (int i=0; i<4; ++i)
	    {
	      for (int j=i; i<4; ++i)
	      {
		if (i==j)
		{
		  vec_spd(k) = mycov(i,j);
		  
		}
		else
		{
		  vec_spd(k) = sr2*mycov(i,j);
		  
		}
	      }
	      
	    }
	    vec_spd.t().print("vec_spd");
	    
	    
	    
      getchar();
      
    }
  }

  
}

