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


///Grassmann Manifolds

///Projection Metric
inline
float
cv_classify_NN::proj_grass(int in_p)
{
  
  p = in_p;
  
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
  load_sub_path  << path << "dim_" << dim << "/grass_points/one-grass-point/scale" << scale_factor << "-shift"<< shift ;
  
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
      
      
      std::stringstream load_Gnp;
      load_Gnp << load_sub_path.str() << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
      //cout <<  action_name << "_" <<  folder_n << endl;
      //#pragma omp critical
      //cout << load_cov_seg.str() << endl;
      
      est_label_video_i = ProjectionMetric_one_video( action_seq_names, test_i, load_sub_path.str(), load_Gnp.str() );
      
      real_labels(c)=act;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_Gnp.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == act)
	{acc++;  }
      }
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
cv_classify_NN::ProjectionMetric_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp)
{
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
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
      
      if (!(action_name=="Run-Side" && folder_n=="001"))
      {
	
	std::stringstream load_Gnp_tr;
	load_Gnp_tr << load_sub_path << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
	
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
  
  
  return est_lab;
  
  
}

///  Binet-Cauchy Metric
inline
float
cv_classify_NN::BC_grass(int in_p)
{
  p = in_p;
  
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
  load_sub_path  << path << "dim_" << dim << "/grass_points/one-grass-point/scale" << scale_factor << "-shift"<< shift ;
  
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
      
      
      std::stringstream load_Gnp;
      load_Gnp << load_sub_path.str() << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
      //cout <<  action_name << "_" <<  folder_n << endl;
      //#pragma omp critical
      //cout << load_cov_seg.str() << endl;
      
      est_label_video_i = BinetCauchyMetric_one_video( action_seq_names, test_i, load_sub_path.str(), load_Gnp.str() );
      
      real_labels(c)=act;
      est_labels(c)=est_label_video_i;
      test_video_list(c) = load_Gnp.str();
      c++;
      
      
      //#pragma omp critical
      {
	if (est_label_video_i == act)
	{acc++;  }
      }
    }
    
  }
  
  
  std::ostringstream save_BC;
  save_BC << "./results/grass_BC_est_labels_p" << p << ".dat";;
  est_labels.save(save_BC.str(), raw_ascii);  
  cout << "Performance  for Binet-Cauchy p " << p << ": " << acc*100/n_test << " %" << endl;
  acc =   acc*100/n_test;
  return acc;
  
}


inline
uword
cv_classify_NN::BinetCauchyMetric_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp)
{
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
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
      
      if (!(action_name=="Run-Side" && folder_n=="001"))
      {
	
	std::stringstream load_Gnp_tr;
	load_Gnp_tr << load_sub_path << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
	
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
  
    return est_lab;
  
}


