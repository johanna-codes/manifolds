#include "omp.h"

inline
cv_classify_NN::cv_classify_NN(const std::string in_path,
			       const std::string in_actionNames,  
			       const float in_scale_factor, 
			       const int in_shift,
			       const int in_dim 
):path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim)
{
  actions.load( actionNames );  
  
  
}


// Log-Euclidean Distance
inline
float
cv_classify_NN::logEucl()
{
  
  int n_actions = actions.n_rows;
  int n_test = 108;  //
  
  float acc;
  acc = 0;
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  mat action_seq_names (108,2);  ///From the dataset description 
  
  
   for (int actID=0; actID<n_actions; ++actID)
    {
      for (int seqID=0; seqID< 12; ++seqID) ///From the dataset description
      {
	action_seq_names(k,0) = actID;
	action_seq_names(k,1) = seqID;
	k++;
    }
  }
  
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "cov_matrices/one-cov-mat/scale" << scale_factor << "-shift"<< shift ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  int c = 0;
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    int actID  = action_seq_names(test_i,0);  
    int seqID  = action_seq_names(test_i,1);
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    
    std::stringstream load_cov;
    load_cov << load_sub_path.str() << "/LogMcov_"  << actions(actID) << "_seq" <<  seqID << ".h5";
    //cout << load_cov.str() << endl;
      

      
      est_label_video_i = logEucl_one_video( action_seq_names, test_i, load_sub_path.str(), load_cov.str() );
      
      real_labels(c) = actID;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_cov.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == actID)
	{acc++;  }
      }
    
  }
  
  real_labels.save("./results/nn_Log_Eucl_real_labels.dat", raw_ascii);
  est_labels.save("./results/nn_Log_Eucl_est_labels.dat", raw_ascii);
  test_video_list.save("nn_Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  return acc*100/n_test;
}


inline
uword
cv_classify_NN::logEucl_one_video(mat action_seq_names, int test_i, std::string load_sub_path, std::string load_cov)
{
  
  mat logMtest_cov;
  logMtest_cov.load(load_cov);
  
  
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  double est_lab;
  
  for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
  {
    
    if (train_i!= test_i)
    {	     
      
      int actID  = action_seq_names(train_i,0);  
      int seqID  = action_seq_names(train_i,1);
      
      std::stringstream load_cov_tr;
      load_cov_tr << load_sub_path << "/LogMcov_"  << actions(actID) << "_seq" <<  seqID << ".h5";
      mat logMtrain_cov;
      logMtrain_cov.load( load_cov_tr.str() );
      dist = norm( logMtest_cov - logMtrain_cov, "fro");
      
	if (dist < tmp_dist)
	{
	  tmp_dist = dist;
	  est_lab = actID;
	}

      
    }
  }
  
  
  return est_lab;
  
}










///Grassmann Manifolds

///Projection Metric
inline
float
cv_classify_NN::proj_grass(int in_p)
{
  
  p = in_p;
  
  int n_actions = actions.n_rows;
  int n_test = 108;  //
  
  float acc;
  acc = 0;
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  mat action_seq_names (108,2);  ///From the dataset description (150)
  
  
   for (int actID=0; actID<n_actions; ++actID)
    {
      for (int seqID=0; seqID< 12; ++seqID) ///From the sataset description
      {
	action_seq_names(k,0) = actID;
	action_seq_names(k,1) = seqID;
	k++;
    }
  }
  
  
  

  
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "/grass_points/one-grass-point/scale" << scale_factor << "-shift"<< shift ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  int c=0;
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    int actID  = action_seq_names(test_i,0);  
    int seqID  = action_seq_names(test_i,1);
    
      
      int tid=omp_get_thread_num();
      uword est_label_video_i;
      
      
      std::stringstream load_Gnp;
      load_Gnp << load_sub_path.str() << "/grass_pt_" << actions(actID) << "_seq" <<  seqID << "_p" << p << ".h5";
      //cout <<  action_name << "_" <<  folder_n << endl;
      //#pragma omp critical
      //cout << load_cov_seg.str() << endl;
      
      est_label_video_i = ProjectionMetric_one_video( action_seq_names, test_i, load_sub_path.str(), load_Gnp.str() );
      
      real_labels(c)=actID;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_Gnp.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == actID)
	{acc++;  }
      }

    
  }
  
  
  std::ostringstream save_PM;
  save_PM << "./results/grass_PM_est_labels_p" << p << ".dat";
  est_labels.save(save_PM.str(), raw_ascii);  
  cout << "Performance for Projection Metric p " << p << ": " << acc*100/n_test << " %" << endl;
  acc =   acc*100/n_test;
  
  
  
  // Uncomment for the best p and save!!!! :???????????
  //    real_labels.save("./results_onesegment/grass_PM_real_labels.dat", raw_ascii);
  //    est_labels.save("./results_onesegment/grass_PM_est_labels.dat", raw_ascii);
  //    test_video_list.save("./results_onesegment/grass_PM_test_video_list.dat", raw_ascii);
  //    cout << "Performance for Projection Metric: " << acc*100/n_test << " %" << endl;
  //    acc =   acc*100/n_test;
  return acc;
}


inline
uword
cv_classify_NN::ProjectionMetric_one_video(mat action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp)
{
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  double est_lab;
  
  for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
  {
    
    if (train_i!= test_i)
    {	     
      
      int actID  = action_seq_names(train_i,0);  
      int seqID  = action_seq_names(train_i,1);
      
      std::stringstream load_Gnp_tr;
      load_Gnp_tr << load_sub_path << "/grass_pt_" << actions(actID) << "_seq" <<  seqID << "_p" << p << ".h5";
      
      mat grass_point_train;
      grass_point_train.load( load_Gnp_tr.str() );
      dist = grass_dist.proj_metric(grass_point_test,grass_point_train, p);
      
      if (dist < tmp_dist)
      {
	tmp_dist = dist;
	est_lab = actID;
	
	
      }  

      
    }
  }
  
  
  return est_lab;
  
  
}
