#include "omp.h"

inline
kth_cv_omp::kth_cv_omp(const std::string in_path,
		       const std::string in_actionNames,  
		       const field<std::string> in_all_people,
		       const float in_scale_factor, 
		       const int in_shift,
		       const int in_scene, //only for kth
		       const int in_dim 
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), dim(in_dim)
{
  actions.load( actionNames );  
  
  
}


/// GMM over each video . Multiples Ng

// Ground Distance (GD) Based on Product of Lie Groups
inline
float
kth_cv_omp::Ground_Distance_GMM(const int Ng)
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; // - person13_handclapping_d3
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  int sc = 1; // = total scenes
  
  mat peo_act(n_test,2);
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      peo_act (k,0) = pe;
      peo_act (k,1) = act;
      k++;
    }
  }
  
  
  //omp_set_num_threads(8); //Use only 8 processors
  // #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe_test  = peo_act (n,0);
    int act_test = peo_act (n,1);
    
    
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
   
    est_label_video_i = GD_gmm( pe_test, act_test, Ng );
    
    real_labels(n)=act_test;
    est_labels(n)=est_label_video_i;
    
    
    //#pragma omp critical
    {
      if (est_label_video_i == act_test)
      {acc++;  }
    }
    
  }
  
  est_labels.t().print();
  cout << endl;
  real_labels.t().print();
  //   real_labels.save("./results_onesegment/Log_Eucl_real_labels.dat", raw_ascii);
  //   est_labels.save("./results_onesegment/Log_Eucl_est_labels.dat", raw_ascii);
  //   test_video_list.save("./results_onesegment/Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance for " <<  Ng << " Gaussians = "  << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
  
}


inline
uword
kth_cv_omp::GD_gmm(int pe_test, int act_test, const int Ng)
{
  
  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_train = 0; pe_train< n_peo; ++pe_train)
  {
    if (pe_train!= pe_test)
    {	     
      
      for (int act_train=0; act_train<n_actions; ++act_train)
      {

	dist = dist_te_tr(pe_test, pe_train, act_test, act_train, Ng);
	//cout << "dist= " <<  dist << endl;

	if (dist < tmp_dist)
	{
	  tmp_dist = dist;
	  est_lab = act_train;
	}
	
      }
    }
  }

  
  return est_lab;
  
}


inline
float
kth_cv_omp::dist_te_tr(int pe_test, int pe_train, int act_test, int act_train, const int Ng)
{
  
  int sc =1;
  std::stringstream load_sub_path;
  load_sub_path  << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
  float theta = 0.5; //See Experiments in original paper
  double tmp_dist_a, tmp_dist_b;

  mat test_Cov,  test_logM_Cov,  train_Cov,  train_logM_Cov;
  
  vec train_Mean, test_Mean;
  
  float dist = 0;
  
  for (int ng_te=1;ng_te<=Ng; ++ng_te)
  {
    
    std::stringstream load_Cov;
    load_Cov << load_sub_path.str() << "/Cov_" << ng_te << "out" << Ng << "_"  << all_people (pe_test) << "_" << actions(act_test) << ".h5";
    
    std::stringstream load_logMCov;
    load_logMCov << load_sub_path.str() << "/logM_Cov_" << ng_te << "out" << Ng << "_"  << all_people (pe_test) << "_" << actions(act_test) << ".h5";
    
    
    std::stringstream load_Mean;
    load_Mean << load_sub_path.str() << "/Means_" << ng_te << "out" << Ng << "_"  << all_people (pe_test) << "_" << actions(act_test) << ".h5";
    
//     cout << load_Cov.str() << endl;
//     cout << load_logMCov.str() << endl;
//     cout << load_Mean.str() << endl;
    
    
    test_Cov.load( load_Cov.str() );
    test_logM_Cov.load( load_logMCov.str() );
    test_Mean.load( load_Mean.str() );
    
//     cout << "Press a Key" << endl;
//     getchar();
    
    
    for (int ng_tr=1;ng_tr<=Ng; ++ng_tr)
    {
      
      std::stringstream load_Covs_tr;
      load_Covs_tr << load_sub_path.str() << "/Cov_" << ng_tr << "out" << Ng << "_" << all_people (pe_train) << "_" << actions(act_train) << ".h5";
      
      std::stringstream load_logM_Covs_tr;
      load_logM_Covs_tr << load_sub_path.str() << "/logM_Cov_" << ng_tr << "out" << Ng << "_"  << all_people (pe_train) << "_" << actions(act_train) << ".h5";
      
      std::stringstream load_Means_tr;
      load_Means_tr << load_sub_path.str() << "/Means_" << ng_tr << "out" << Ng << "_" << all_people (pe_train) << "_" << actions(act_train) << ".h5";
      
      train_Cov.load( load_Covs_tr.str() ) ;
      train_logM_Cov.load( load_logM_Covs_tr.str() );
      train_Mean.load( load_Means_tr.str() );
      
      
//       cout << load_Covs_tr.str()  << endl;
//       cout << load_Means_tr.str() << endl;
//       cout << load_logM_Covs_tr.str() << endl;
//       
//       cout << "Press a Key" << endl;
//       getchar();
      
      
      vec subs;
      mat sum;
      
      subs = ( test_Mean - train_Mean );
      sum = inv( test_Cov ) + inv( train_Cov );
      
      //cout << "tmp_dist_a" << endl;
      tmp_dist_a =sqrt( as_scalar( subs.t()*sum*subs ) );
      
      //cout << "tmp_dist_b" << endl;
      tmp_dist_b = norm( test_logM_Cov - train_logM_Cov, "fro");
      
      //cout << "dist" << endl;
      float tmp_dist = (1-theta)*tmp_dist_a + theta*tmp_dist_b;
      
      dist+= tmp_dist;
      
    }
    
  }
  
  
  dist = dist/(Ng*Ng);
  return dist;
  
}

/// One Covariance and one mean per video 

// Ground Distance (GD) Based on Product of Lie Groups
inline
float
kth_cv_omp::Ground_Distance()
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; // - person13_handclapping_d3
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  int sc = 1; // = total scenes
  
  mat peo_act(n_test,2);
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      peo_act (k,0) = pe;
      peo_act (k,1) = act;
      k++;
    }
  }
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  // #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_Covs;
    load_Covs << load_sub_path.str() << "/Cov_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    std::stringstream load_logMCovs;
    load_logMCovs << load_sub_path.str() << "/logM_Cov_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    
    std::stringstream load_Means;
    load_Means << load_sub_path.str() << "/Means_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = GD_one_video( pe, load_sub_path.str(), load_Covs.str(), load_logMCovs.str(), load_Means.str() );
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
    
    
    //#pragma omp critical
    {
      if (est_label_video_i == act)
      {acc++;  }
    }
    
  }
  
  est_labels.t().print();
  //   real_labels.save("./results_onesegment/Log_Eucl_real_labels.dat", raw_ascii);
  //   est_labels.save("./results_onesegment/Log_Eucl_est_labels.dat", raw_ascii);
  //   test_video_list.save("./results_onesegment/Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
  
}


inline
uword
kth_cv_omp::GD_one_video(int pe_test, std::string load_sub_path, std::string load_Covs, std::string load_logMCovs, std::string load_Means)
{
  
  float theta = 0.5; //See Experiments in original paper
  mat test_Cov;
  test_Cov.load(load_Covs);
  
  mat test_logM_Cov;
  test_logM_Cov.load(load_logMCovs);
  
  vec test_Mean;
  test_Mean.load(load_Means);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist, tmp_dist_a, tmp_dist_b;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout << " " << all_people (pe_tr) << endl;
      
      
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  
	  
	  std::stringstream load_Covs_tr;
	  load_Covs_tr << load_sub_path << "/Cov_" << all_people (pe_tr) << "_" << actions(act) << ".h5";
	  
	  std::stringstream load_logM_Covs_tr;
	  load_logM_Covs_tr << load_sub_path << "/logM_Cov_" << all_people (pe_tr) << "_" << actions(act) << ".h5";
	  
	  std::stringstream load_Means_tr;
	  load_Means_tr << load_sub_path << "/Means_" << all_people (pe_tr) << "_" << actions(act) << ".h5";
	  
	  
	  
	  mat train_Cov;
	  mat train_logM_Cov;
	  vec train_Mean;
	  
	  train_Cov.load( load_Covs_tr.str() ) ;
	  train_logM_Cov.load( load_logM_Covs_tr.str() );
	  train_Mean.load( load_Means_tr.str() );
	  
	  vec subs;
	  mat sum;
	  
	  subs = ( test_Mean - train_Mean );
	  sum = inv( test_Cov ) + inv( train_Cov );
	  
	  //cout << "tmp_dist_a" << endl;
	  tmp_dist_a =sqrt( as_scalar( subs.t()*sum*subs ) );
	  
	  //cout << "tmp_dist_b" << endl;
	  tmp_dist_b = norm( test_logM_Cov - train_logM_Cov, "fro");
	  
	  //cout << "dist" << endl;
	  dist = (1-theta)*tmp_dist_a + theta*tmp_dist_b;
	  //cout << "dist= " <<  dist << endl;
	  
	  
	  if (dist < tmp_dist)
	  {
	    tmp_dist = dist;
	    est_lab = act;
	  }
	  
	}
      }
    }
  }
  
  
  return est_lab;
  
}





