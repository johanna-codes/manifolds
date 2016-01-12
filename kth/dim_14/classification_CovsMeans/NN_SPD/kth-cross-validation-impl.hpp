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


// Log-Euclidean Distance
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
  
  est_labels.t().print();
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



