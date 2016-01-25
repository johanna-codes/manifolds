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


/// Log-Euclidean Distance
inline
float
kth_cv_omp::logEucl()
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
    load_sub_path  << path << "covs_means_matrices/CovMeans/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
  //omp_set_num_threads(8); //Use only 8 processors
  #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
 
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_CovMeans;
    load_CovMeans << load_sub_path.str() << "/LogM_CovMeans_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = logEucl_one_video( pe, load_sub_path.str(), load_CovMeans.str());
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
 
    
    #pragma omp critical
    {
    if (est_label_video_i == act)
    {acc++;  }
    }
    
  }
  
//  est_labels.t().print();
//   real_labels.save("./results_onesegment/Log_Eucl_real_labels.dat", raw_ascii);
//   est_labels.save("./results_onesegment/Log_Eucl_est_labels.dat", raw_ascii);
//   test_video_list.save("./results_onesegment/Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
  
}


inline
uword
kth_cv_omp::logEucl_one_video(int pe_test, std::string load_sub_path, std::string load_CovMeans)
{
  //wall_clock timer;
  //timer.tic();
  
  //cout << load_CovMeans << endl;
  mat logMtest_CovMeans;
  logMtest_CovMeans.load(load_CovMeans);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout << " " << all_people (pe_tr);
      
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_CovMeans_tr;
	   load_CovMeans_tr << load_sub_path << "/LogM_CovMeans_" <<  all_people (pe_tr) << "_" << actions(act) << ".h5";
	   mat logMtrain_CovMeans;
	   logMtrain_CovMeans.load( load_CovMeans_tr.str() );
	   dist = norm( logMtest_CovMeans - logMtrain_CovMeans, "fro");
	   //logMtest_CovMeans.print();
	   //logMtrain_CovMeans.print();
	   //cout << dist << endl;
	   //getchar();	    
	   
	   
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



/// Log-Euclidean Distance. GMM
// GMM over each video . Multiples Ng

inline 
float 
kth_cv_omp::logEucl_GMM( const int Ng )//Ground Distance 1 with GMM
{
   int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  float acc;
  acc = 0;
  
  int n_test = n_peo*n_actions*total_scenes; 
  
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
  
  //est_labels.t().print();
  //cout << endl;
  //real_labels.t().print();
  //   real_labels.save("./results_onesegment/Log_Eucl_real_labels.dat", raw_ascii);
  //   est_labels.save("./results_onesegment/Log_Eucl_est_labels.dat", raw_ascii);
  //   test_video_list.save("./results_onesegment/Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance for " <<  Ng << " Gaussians = "  << acc*100/n_test << " %" << endl;
  
  return acc*100/n_test;
 
  
}



inline 
uword 
kth_cv_omp::GD_gmm(int pe_test, int act_test, const int Ng)//Ground Distance 1
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

	if (dist < tmp_dist) // Finding the minimun distance among all people in training set.
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

  mat test_Cov,   train_Cov;
  
  vec train_Mean, test_Mean;
  
  float dist, tmp_dist;
  tmp_dist = datum::inf;
  

  
  
  for (int ng_te=1;ng_te<=Ng; ++ng_te)
  {
    
    std::stringstream load_Cov;
    load_Cov << load_sub_path.str() << "/Cov_" << ng_te << "out" << Ng << "_"  << all_people (pe_test) << "_" << actions(act_test) << ".h5";
    
    
    std::stringstream load_Mean;
    load_Mean << load_sub_path.str() << "/Means_" << ng_te << "out" << Ng << "_"  << all_people (pe_test) << "_" << actions(act_test) << ".h5";
    
//     cout << load_Cov.str() << endl;
//     cout << load_logMCov.str() << endl;
//     cout << load_Mean.str() << endl;
    
    
    test_Cov.load( load_Cov.str() );
    test_Mean.load( load_Mean.str() );
    
//     cout << "Press a Key" << endl;
//     getchar();
    
    
    for (int ng_tr=1;ng_tr<=Ng; ++ng_tr)
    {
      
      std::stringstream load_Covs_tr;
      load_Covs_tr << load_sub_path.str() << "/Cov_" << ng_tr << "out" << Ng << "_" << all_people (pe_train) << "_" << actions(act_train) << ".h5";
      
    
      
      std::stringstream load_Means_tr;
      load_Means_tr << load_sub_path.str() << "/Means_" << ng_tr << "out" << Ng << "_" << all_people (pe_train) << "_" << actions(act_train) << ".h5";
      
      train_Cov.load( load_Covs_tr.str() ) ;
      train_Mean.load( load_Means_tr.str() );
      
      
      mat test_logM_CovMeans = get_emb_LogCov (test_Cov, test_Mean);
      mat train_logM_CovMeans = get_emb_LogCov (train_Cov, train_Mean);   
      
      dist = norm( test_logM_CovMeans - train_logM_CovMeans, "fro");
      
      if (dist < tmp_dist) 
	{
	  tmp_dist = dist;
	}
 

      
    }
    
  }
  
  
  return dist; // returning the minimum distance to the Mixture of Gaussians of pe_train
  
}




//Embedded Log Covariance Matrix + Matrix Logarithm
inline
mat
kth_cv_omp::get_emb_LogCov(mat cov_i, vec mean_i)
{
  //cout << "DIM " << dim << endl;
  double THRESH = 0.000001;
  mat CovMean = zeros(dim+1,dim+1);
  mat cov_i_2 = cov_i + mean_i*mean_i.t();
  CovMean.submat( 0, 0, dim-1, dim-1) = cov_i_2;    
  CovMean.submat( 0, dim, dim-1, dim )  = mean_i;
  CovMean.submat(dim,0,dim,dim-1) = mean_i.t();
  CovMean(dim,dim) = 1;
  
  CovMean = 0.5*(CovMean + CovMean.t());
  vec D;
  mat V;
  eig_sym(D, V, CovMean);
  uvec q1 = find(D < THRESH);
  
  if (q1.n_elem>0)
  {
    for (uword pos = 0; pos < q1.n_elem; ++pos)
    {
      D( q1(pos) ) = THRESH;
      
    }
    
    CovMean = V*diagmat(D)*V.t();  
    
  }
  
  eig_sym(D, V, CovMean);
  mat log_M = V*diagmat( log(D) )*V.t();
  
  
  
  
  
  
  
  return log_M;
}
