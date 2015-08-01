#include "omp.h"

inline
cv_classify_NN::cv_classify_NN(const std::string in_path,
			       const std::string in_path_dataset,
			       const std::string in_actionNames,  
			       const int in_scale_factor, 
			       const int in_shift,
			       const int in_dim 
):path(in_path), path_dataset(in_path_dataset), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim)
{
  actions.load( actionNames );  
  
  
}


// Log-Euclidean Distance
inline
void
cv_classify_NN::logEucl()
{
  
  int n_actions = actions.n_rows;
  
  float acc;
  acc = 0;
  
  
  int n_test = 150 - 1;  //Problem with Run-Side_001_dim14 
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  field <std::string> action_seq_names (150,3);  ///From the dataset description (150)
  
  
  for (int act=0; act<n_actions; ++act)
  {
    std::stringstream  ss_folders;
    ss_folders << path_dataset << actions (act) << "/list_folders.txt";
    field <std::string> list_folders;
    list_folders.load( ss_folders.str() );
    int n_folders = list_folders.n_rows;
    //cout <<  actions (act) << endl;
    //list_folders.print();
    for (int i=0; i< n_folders; ++i)
    {
      
      std::ostringstream ss_action;
      ss_action << act;
      
      action_seq_names(k,0) = actions(act); //Action
      action_seq_names(k,1) = list_folders(i); //Video Sequence
      action_seq_names(k,2) = ss_action.str();; //Action index
      k++;
    }
  }
  
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "dim_" << dim << "/cov_matrices/one-cov-mat/scale" << scale_factor << "-shift"<< shift ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  int c=0;
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    std::string action_name = action_seq_names(test_i,0);   
    std::string folder_n    = action_seq_names(test_i,1);
    int act  = atoi( action_seq_names(test_i,2).c_str() );
    
    if (!(action_name=="Run-Side" && folder_n=="001"))
    {
      
      
      int tid=omp_get_thread_num();
      uword est_label_video_i;
      
      
      std::stringstream load_cov;
      load_cov << load_sub_path.str() << "/LogMcov_" <<  action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
      
      cout <<  action_name << "_" <<  folder_n << endl;
      //#pragma omp critical
      //cout << load_cov_seg.str() << endl;
      
      est_label_video_i = logEucl_one_video( action_seq_names, test_i, load_sub_path.str(), load_cov.str() );
      
      real_labels(c)=act;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_cov.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == act)
	{acc++;  }
      }
    }
    
  }
  
  real_labels.save("nn_Log_Eucl_real_labels.dat", raw_ascii);
  est_labels.save("nn_Log_Eucl_est_labels.dat", raw_ascii);
  test_video_list.save("nn_Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
}


inline
uword
cv_classify_NN::logEucl_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_cov)
{

  mat logMtest_cov;
  logMtest_cov.load(load_cov);
  
  
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  double est_lab;
  
  for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
  {
    
    int act  = atoi( action_seq_names(train_i,2).c_str() );
    if (train_i!= test_i)
    {	     
      
      std::string action_name = action_seq_names(train_i,0);   
      std::string folder_n    = action_seq_names(train_i,1);
      
      std::stringstream load_cov_tr;
      load_cov_tr << load_sub_path << "/LogMcov_" <<  action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
      
      mat logMtrain_cov;
      logMtrain_cov.load( load_cov_tr.str() );
      
      dist = norm( logMtest_cov - logMtrain_cov, "fro");
      
      if (dist < tmp_dist)
      {
	tmp_dist = dist;
	est_lab = act;
      }
      
    }
  }
  
  
  return est_lab;
  
}




// Stein Divergence 

inline
void
cv_classify_NN::SteinDiv()
{
  
  int n_actions = actions.n_rows;
  
  float acc;
  acc = 0;
  
  
  int n_test = 150 - 1;  //Problem with Run-Side_001_dim14 
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  field <std::string> action_seq_names (150,3);  ///From the dataset description (150)
  
  
  for (int act=0; act<n_actions; ++act)
  {
    std::stringstream  ss_folders;
    ss_folders << path_dataset << actions (act) << "/list_folders.txt";
    field <std::string> list_folders;
    list_folders.load( ss_folders.str() );
    int n_folders = list_folders.n_rows;
    //cout <<  actions (act) << endl;
    //list_folders.print();
    for (int i=0; i< n_folders; ++i)
    {
      
      std::ostringstream ss_action;
      ss_action << act;
      
      action_seq_names(k,0) = actions(act); //Action
      action_seq_names(k,1) = list_folders(i); //Video Sequence
      action_seq_names(k,2) = ss_action.str();; //Action index
      k++;
    }
  }
  
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "dim_" << dim << "/cov_matrices/one-cov-mat/scale" << scale_factor << "-shift"<< shift ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  int c=0;
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    std::string action_name = action_seq_names(test_i,0);   
    std::string folder_n    = action_seq_names(test_i,1);
    int act  = atoi( action_seq_names(test_i,2).c_str() );
    
    if (!(action_name=="Run-Side" && folder_n=="001"))
    {
      
      
      int tid=omp_get_thread_num();
      uword est_label_video_i;
      
      
      std::stringstream load_cov;
      load_cov << load_sub_path.str() << "/cov_" <<  action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
      
      cout <<  action_name << "_" <<  folder_n << endl;
      //#pragma omp critical
      //cout << load_cov_seg.str() << endl;
      
      est_label_video_i = SteinDiv_one_video( action_seq_names, test_i, load_sub_path.str(), load_cov.str() );
      
      real_labels(c)=act;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_cov.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == act)
	{acc++;  }
      }
    }
    
  }
  
  real_labels.save("nn_Stein_div_real_labels.dat", raw_ascii);
  est_labels.save("nn_Stein_div_est_labels.dat", raw_ascii);
  test_video_list.save("nn_Stein_div_test_video_list.dat", raw_ascii);
  cout << "Performance for SteinDiv " << acc*100/n_test << " %" << endl;
  
  
}



inline
uword
cv_classify_NN::SteinDiv_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_cov)
{

  mat test_cov;
  test_cov.load(load_cov);
  
  
  
  double dist_stein, tmp_dist;
  tmp_dist = datum::inf;
  
  double est_lab;
  
  for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
  {
    
    int act  = atoi( action_seq_names(train_i,2).c_str() );
    if (train_i!= test_i)
    {	     
      
      std::string action_name = action_seq_names(train_i,0);   
      std::string folder_n    = action_seq_names(train_i,1);
      
      std::stringstream load_cov_tr;
      load_cov_tr << load_sub_path << "/cov_" <<  action_name << "_" <<  folder_n << "_dim" << dim  << ".h5";
      
      mat train_cov;
      train_cov.load( load_cov_tr.str() );
      
      double det_op1 = det( diagmat( (test_cov + train_cov)/2 ) );
      double det_op2 = det( diagmat( ( test_cov%train_cov ) ) );
      dist_stein =  log( det_op1 ) - 0.5*log( det_op2 ) ;
	  
      
      
      if (dist_stein < tmp_dist)
      {
	tmp_dist = dist_stein;
	est_lab = act;
      }
      
    }
  }
  
  
  return est_lab;
 
  
}



/*


///Grassmann Manifolds

///Projection Metric
inline
float
cv_classify_NN::proj_grass(int in_p)
{
  
  p = in_p;
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
  load_sub_path  << path << "grass_points/kth-grass-point-one-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
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
    
    std::stringstream load_Gnp;
    load_Gnp << load_sub_path.str() << "/grass_pt_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << "_p" << p<< ".h5";
    
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = ProjectionMetric_one_video( pe, load_sub_path.str(), load_Gnp.str(), p);
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
    
    
    #pragma omp critical
    {
      if (est_label_video_i == act)
      {acc++;  }
    }
    
  }
  
  //    real_labels.save("./results_onesegment/grass_PM_real_labels.dat", raw_ascii);
  //    est_labels.save("./results_onesegment/grass_PM_est_labels.dat", raw_ascii);
  //    test_video_list.save("./results_onesegment/grass_PM_test_video_list.dat", raw_ascii);
  //    cout << "Performance for Projection Metric: " << acc*100/n_test << " %" << endl;
  //    acc =   acc*100/n_test;
  return acc;
}


inline
uword
cv_classify_NN::ProjectionMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp, int p)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
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
	  
	  std::stringstream load_Gnp_tr;
	  load_Gnp_tr << load_sub_path << "/grass_pt_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim << "_p" << p << ".h5";
	  
	  
	  mat grass_point_train;
	  grass_point_train.load( load_Gnp_tr.str() );
	  
	  dist = grass_dist.proj_metric(grass_point_test,grass_point_train, p);
	  
	  
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

///Binet-Cauchy Metric
inline
float
cv_classify_NN::BC_grass(int in_p)
{
  p = in_p;
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
  load_sub_path  << path << "grass_points/kth-grass-point-one-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
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
    
    std::stringstream load_Gnp;
    load_Gnp << load_sub_path.str() << "/grass_pt_" << all_people (pe) << "_" << actions(act) << "_dim" << dim << "_p" << p  << ".h5";
    
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = BinetCauchyMetric_one_video( pe, load_sub_path.str(), load_Gnp.str());
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
    
    
    
    #pragma omp critical
    {
      if (est_label_video_i == act)
      {acc++;  }
    }
    
  }
  
  //    real_labels.save("./results_onesegment/grass_BC_real_labels.dat", raw_ascii);
  //    est_labels.save("./results_onesegment/grass_BC_est_labels.dat", raw_ascii);
  //    test_video_list.save("./results_onesegment/grass_BC_test_video_list.dat", raw_ascii);
  //    cout << "Performance for Binet-Cauchy: " << acc*100/n_test << " %" << endl;  
  //    acc =   acc*100/n_test;
  return acc;
  
}


inline
uword
cv_classify_NN::BinetCauchyMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
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
	  
	  std::stringstream load_Gnp_tr;
	  load_Gnp_tr << load_sub_path << "/grass_pt_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim << "_p" << p << ".h5";
	  
	  
	  mat grass_point_train;
	  grass_point_train.load( load_Gnp_tr.str() );
	  
	  
	  dist = grass_dist.BC_metric(grass_point_test,grass_point_train);
	  
	  
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

*/