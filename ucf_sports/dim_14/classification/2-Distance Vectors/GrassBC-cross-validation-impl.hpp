#include "omp.h"

inline
cv_dist_vector_GrassBC::cv_dist_vector_GrassBC(const std::string in_path,
					       const std::string in_path_dataset,
					       const std::string in_actionNames,  
					       const int in_dim,
					       const int in_p
):path(in_path), path_dataset(in_path_dataset), actionNames(in_actionNames), dim(in_dim), p(in_p)
{
  actions.load( actionNames );  
  
  
}



inline
void
cv_dist_vector_GrassBC::train(int tr_scale, int tr_shift )
{
  distances(tr_scale, tr_shift);
  svm_train(); 
  
}

inline
void
cv_dist_vector_GrassBC::svm_train()
{
  
  int n_actions = actions.n_rows;
  int n_test = 150 - 1;  //Problem with Run-Side_001_dim14 
  int n_dim = n_test -1;
  int k = 0;
  
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
  
  
  //****************************************
  //#pragma omp parallel for 
  for (int seq_ts=0; seq_ts<action_seq_names.n_rows; ++seq_ts) 
  {
    cout << seq_ts << endl;
    std::string action_name_ts = action_seq_names(seq_ts,0);   
    std::string folder_n_ts    = action_seq_names(seq_ts,1);
    
    if (!(action_name_ts=="Run-Side" && folder_n_ts=="001"))
    {
      
      fmat training_data;
      fvec lab;
      training_data.zeros(n_dim,n_test);
      lab.zeros(n_test);
      int k = 0;
      
      for (int seq_tr=0; seq_tr<action_seq_names.n_rows; ++seq_tr) //Problem with Run-Side_001_dim14 
      {
	std::string action_name_tr = action_seq_names(seq_tr,0);   
	std::string folder_n_tr    = action_seq_names(seq_tr,1);
	int act_tr  = atoi( action_seq_names(seq_tr,2).c_str() );
	
	if (!(action_name_tr=="Run-Side" && folder_n_tr=="001"))
	{
	  fvec dist_vector;
	  std::stringstream load_vec_dist;
	  load_vec_dist << "./GrassBC/dist_vector_" << action_name_tr << "_" <<  folder_n_tr << "_dim" << dim  << "_p" << p  << ".h5";
	  dist_vector.load( load_vec_dist.str() );
	  training_data.col(k) = dist_vector;
	  lab(k) = act_tr;
	  ++k;
	}
      }


	
	//Training the model with OpenCV
	//#pragma omp critical
	//cout << "Using SVM to train run " << seq_ts+1 << endl;
	//cout << "Preparing data to train the data" << endl;
	cv::Mat cvMatTraining(n_test, n_dim, CV_32FC1);
	float fl_labels[n_test] ;
	
	//cout << "Preparing data" << endl;
	for (uword m=0; m<n_test; ++m)
	{
	  for (uword d=0; d<n_dim; ++d)
	  {
	    cvMatTraining.at<float>(m,d) = training_data(d,m); 
	    //cout << " OpenCV: " << cvMatTraining.at<float>(m,d) << " - Arma: " <<training_data(d,m); 
	  }
	  fl_labels[m] = lab(m);
	  //cout <<" OpenCVLabel: " <<  fl_labels[m] << " ArmaLabel: " << labels(m) << endl;
	}
	
	cv::Mat cvMatLabels(n_test, 1, CV_32FC1,fl_labels );
	
	//cout << "Setting parameters" << endl;
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR; 
	//params.gamma = 1;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER,  (int)1e7, 1e-6);
	//params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
	
	// Train the SVM
	//cout << "...Training" << endl;
	CvSVM SVM;
	SVM.train( cvMatTraining , cvMatLabels, cv::Mat(), cv::Mat(), params);
	//#pragma omp critical
	{
	std::stringstream save_svm_model;
	save_svm_model << "./svm_models/GrassBC_run_" << seq_ts+1;
	//cout << "Saving" << endl;
	SVM.save( save_svm_model.str().c_str() );
	}
      }
    }
  }

inline
float
cv_dist_vector_GrassBC::test(int ts_scale, int ts_shift)
{
  
  int n_actions = actions.n_rows;
  int k = 0;
  
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
  
  
  int n_test = 150 - 1;  //Problem with Run-Side_001_dim14 
  int n_dim = n_test -1;
  fvec dist_vector;
  
  float acc=0;
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int j =0;
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "dim_" << dim << "/grass_points/one-grass-point/scale" << ts_scale << "-shift"<< ts_shift ;
  
  
  
  
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    
    std::string action_name = action_seq_names(test_i,0);   
    std::string folder_n    = action_seq_names(test_i,1);
    int act_ts  = atoi( action_seq_names(test_i,2).c_str() );
    
    if (!(action_name=="Run-Side" && folder_n=="001"))
    {
      CvSVM SVM;
      std::stringstream load_svm_model;
      load_svm_model << "./svm_models/GrassBC_run_" << test_i+1;
      SVM.load( load_svm_model.str().c_str() );
      
      vec test_dist;
      
      std::stringstream load_Gnp;
      load_Gnp << load_sub_path.str() << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
     
      
      test_dist = dist_one_video( action_seq_names, test_i, load_sub_path.str(), load_Gnp.str() );
      
      //test_dist.print("");
      
      cv::Mat cvMatTesting_onevideo(1, n_dim, CV_32FC1);
      
      for (uword d=0; d<n_dim; ++d)
      {
	cvMatTesting_onevideo.at<float>(0,d) = test_dist(d); 
	
      }
      
      float response = SVM.predict(cvMatTesting_onevideo, true);
      
      std::stringstream test_video_name;
      test_video_name << action_name << "_" <<  folder_n ;
      
      
      //cout << "response " << response << endl;
      real_labels(j) = act_ts;
      est_labels(j) = response;
      test_video_list(j) = test_video_name.str();
      j++;
      
      //cout << act_ts << " " << response << endl;
      if (response == act_ts)  {
	acc++;
      }
      
      std::stringstream main_save;
      main_save << "./svm_results/GrassBC_scale" <<  ts_scale << "-shift"<< ts_shift << "-p" << p;
      
      
      std::stringstream save1, save2,save3;
      
      save1 << main_save.str() << "_real_labels.dat";
      save2 << main_save.str() << "_est_labels.dat";
      save3 << main_save.str() << "_test_video_list.dat";
      
      real_labels.save(save1.str(), raw_ascii);
      est_labels.save( save2.str(), raw_ascii);
      test_video_list.save(save3.str(), raw_ascii); 
      
    }
    
  }
  acc = acc*100/(n_test);
  cout << "Performance for Grassmann Binet-Cauchy Metric: " <<  acc << " %" << endl;
  
  return acc;
}


///

inline
void
cv_dist_vector_GrassBC::distances(int scale_factor, int shift)
{
  
  int n_actions = actions.n_rows;
  
  int scale2 = scale_factor;
  int shift2 = shift;
  
  int n_test =150 - 1; //  //Problem with Run-Side_001_dim14 
  
  
  
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
  
    
  #pragma omp parallel for 
  for (int test_i = 0; test_i< action_seq_names.n_rows; ++test_i)
  {
    std::string action_name = action_seq_names(test_i,0);   
    std::string folder_n    = action_seq_names(test_i,1);
    int act  = atoi( action_seq_names(test_i,2).c_str() );
    
    //cout << action_name << " " << folder_n << endl;
    //getchar();
    
    if (!(action_name=="Run-Side" && folder_n=="001"))
    {
      int tid = omp_get_thread_num();
      vec dist_video_i;
      
      
      std::stringstream load_Gnp;
      load_Gnp << load_sub_path.str() << "/grass_pt_" << action_name << "_" <<  folder_n << "_dim" << dim << "_p" << p << ".h5";
      
      
      dist_video_i = dist_one_video( action_seq_names, test_i, load_sub_path.str(), load_Gnp.str() );
       
      std::stringstream save_vec_dist;
      save_vec_dist << "./GrassBC/dist_vector_" << action_name << "_" <<  folder_n << "_dim" << dim  << "_p" << p  << ".h5";
      
      #pragma omp critical
      dist_video_i.save(save_vec_dist.str(), hdf5_binary);
    }
  }
}


inline
vec
cv_dist_vector_GrassBC::dist_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp)
{
  grass_metric grass_dist;
  
  mat grass_point_test;
  grass_point_test.load(load_Gnp);

  
  double tmp_dist;
  vec dist;
  int num_dist = action_seq_names.n_rows - 1; 
  num_dist = num_dist - 1; // Problem with Run-Side_001_dim14 
  dist.zeros(num_dist);
  
  double est_lab;
  int k = 0;
  
  for (int train_i = 0; train_i< action_seq_names.n_rows; ++train_i)
  {
    
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
      
	
	tmp_dist = grass_dist.BC_metric(grass_point_test,grass_point_train);
	dist(k) = tmp_dist;
	++k;
      }
    }
  }
  
  return dist;
}


