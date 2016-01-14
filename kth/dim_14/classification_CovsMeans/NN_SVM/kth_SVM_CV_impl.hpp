#include "omp.h"

inline
kth_cv_distNN_svm::kth_cv_distNN_svm(const std::string in_path,
				     const std::string in_actionNames,  
				     const field<std::string> in_all_people,
				     const int in_scene, //only for kth
				     const int in_dim,
				     const int in_GD_type
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), total_scenes(in_scene), dim(in_dim), GD_type(in_GD_type)
{
  actions.load( actionNames );  
  
  
}



inline
void
kth_cv_distNN_svm::train( int tr_scale, int tr_shift )
{
  cout << "Distances"<< endl;
  distances(tr_scale, tr_shift);
  cout << "Training" << endl;
  svm_train(); 
  cout << "End Training" << endl;
  
}

inline
void
kth_cv_distNN_svm::svm_train()
{
  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = (n_peo-1)*n_actions*total_scenes; // - person13_handclapping_d3
  int n_dim = n_test;
  int sc = 1; // = total scenes
  fvec dist_vector;
  
  
  for (int pe_ts=0; pe_ts<n_peo; ++pe_ts)
  {
    
    fmat training_data;
    fvec lab;
    training_data.zeros(n_dim,n_test);
    lab.zeros(n_test);
    int k=0;
    for (int pe_tr=0; pe_tr<n_peo; ++pe_tr)
    {
      if(pe_tr!=pe_ts)
	
	for (int act=0; act<n_actions; act++)
	{
	  
	  std::stringstream load_vec_dist;
	  load_vec_dist << "./GD_2/dist_vector_" << all_people (pe_tr) << "_" << actions(act) << ".h5" ;
	  dist_vector.load( load_vec_dist.str() );
	  training_data.col(k) = dist_vector;
	  lab(k) = act;
	  ++k;
	}
    }
    
    //Training the model with OpenCV
    cout << "Using SVM to train run " << pe_ts+1 << endl;
    //cout << "Preparing data to train the data" << endl;
    cv::Mat cvMatTraining(n_test, n_dim, CV_32FC1);
    float fl_labels[n_test] ;
    
    
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
    //cout << "Training" << endl;
    CvSVM SVM;
    SVM.train( cvMatTraining , cvMatLabels, cv::Mat(), cv::Mat(), params);
    
    
    std::stringstream save_svm_model;
    save_svm_model << "./svm_models/GD_2_svm_run_" << pe_ts+1;
    SVM.save( save_svm_model.str().c_str() );
  }
}


inline
void
kth_cv_distNN_svm::test(int ts_scale, int ts_shift)
{
  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = (n_peo-1)*n_actions*total_scenes; // - person13_handclapping_d3
  int n_dim = n_test;
  int sc = 1; // = total scenes
  fvec dist_vector;
  
  float acc=0;
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_peo*n_actions);
  
  
  real_labels.zeros(n_peo*n_actions);
  est_labels.zeros(n_peo*n_actions);
  
  int j =0;
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << ts_scale << "-shift"<< ts_shift ;
  
  
  
  
  
  for (int pe_ts=0; pe_ts<n_peo; ++pe_ts)
  {
    
    CvSVM SVM;
    
    std::stringstream load_svm_model;
    load_svm_model << "./svm_models/GD_2_svm_run_" << pe_ts+1;
    SVM.load( load_svm_model.str().c_str() );
    
    for (int act_ts =0; act_ts<n_actions; ++act_ts)
    {
      vec test_dist;
      
      std::stringstream load_Covs_ts;
      load_Covs_ts << load_sub_path.str() << "/Cov_" << all_people (pe_ts) << "_" << actions(act_ts) << ".h5";
      
      
      std::stringstream load_logM_Covs_ts;
      load_logM_Covs_ts << load_sub_path.str() << "/logM_Cov_" << all_people (pe_ts) << "_" << actions(act_ts) << ".h5";
      
      std::stringstream load_Means_ts;
      load_Means_ts << load_sub_path.str() << "/Means_" << all_people (pe_ts) << "_" << actions(act_ts) << ".h5";
      
      
      
      
      
      std::stringstream load_cov;
      load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe_ts) << "_" << actions(act_ts) << "_dim" << dim  << ".h5";
      
      
      test_dist = dist_one_video( pe_ts, load_sub_path.str(), load_Covs_ts.str(), load_logM_Covs_ts.str(), load_Means_ts.str() );	
      
      
      
      cv::Mat cvMatTesting_onevideo(1, n_dim, CV_32FC1);
      
      for (uword d=0; d<n_dim; ++d)
      {
	cvMatTesting_onevideo.at<float>(0,d) = test_dist(d); 
	
      }
      
      float response = SVM.predict(cvMatTesting_onevideo, true);
      
      std::stringstream test_video_name;
      test_video_name << all_people(pe_ts) << "_" << actions(act_ts);
      
      
      //cout << "response " << response << endl;
      real_labels(j) = act_ts;
      est_labels(j) = response;
      test_video_list(j) = test_video_name.str();
      j++;
      
      if (response == act_ts)  {
	acc++;
	
      }
      
    }
    
    //       std::stringstream main_save;
    //       main_save << "./svm_results_2/LogEucl_scale" <<  ts_scale << "-shift"<< ts_shift;
    //       
    //       
    //       std::stringstream save1, save2,save3;
    //       
    //       save1 << main_save.str() << "_real_labels.dat";
    //       save2 << main_save.str() << "_est_labels.dat";
    //       save3 << main_save.str() << "_test_video_list.dat";
    //       
    //       real_labels.save(save1.str(), raw_ascii);
    //       est_labels.save( save2.str(), raw_ascii);
    //       test_video_list.save(save3.str(), raw_ascii); 
    
  }
  cout << "Performance for GD_2 = : " << acc*100/(n_peo*n_actions) << " %" << endl;
}


///

inline
void
kth_cv_distNN_svm::distances(int scale_factor, int shift)
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int scale2 = scale_factor;
  int shift2 = shift;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; // - person13_handclapping_d3
  
  
  
  
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
  load_sub_path  << path << "covs_means_matrices_vectors/CovMeans/sc" << sc << "/scale" << scale2 << "-shift"<< shift2 ;
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    int tid=omp_get_thread_num();
    vec dist_video_i;
    
    
    std::stringstream load_Covs;
    load_Covs << load_sub_path.str() << "/Cov_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    std::stringstream load_logMCovs;
    load_logMCovs << load_sub_path.str() << "/logM_Cov_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    
    std::stringstream load_Means;
    load_Means << load_sub_path.str() << "/Means_" << all_people (pe) << "_" << actions(act) << ".h5";
    
    
    //cout << "Distance Between " <<  all_people (pe) << "_" <<  actions(act) << "& "; 
    
    dist_video_i = dist_one_video( pe, load_sub_path.str(), load_Covs.str(),load_logMCovs.str(), load_Means.str() );
    //getchar();
    //dist_video_i = dist_video_i/norm(dist_video_i,2);
    
    //save dist_video_i person, action  
    std::stringstream save_vec_dist;
    save_vec_dist << "./GD_2/dist_vector_" << all_people (pe) << "_" << actions(act) << ".h5" ;
    
    
    //#pragma omp critical
    
    
    dist_video_i.save(save_vec_dist.str(), hdf5_binary);
    
  }
  
}


inline
vec
kth_cv_distNN_svm::dist_one_video(int pe_test, std::string load_sub_path, std::string load_Covs, std::string load_logMCovs, std::string load_Means)
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
  
  
  double  tmp_dist_a, tmp_dist_b;
  vec dist;
  int num_dist = (n_peo-1)*n_actions;
  dist.zeros(num_dist);
  
  
  int k=0;
  
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout <<  all_people (pe_tr) << " ";
      
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
	
	
	
	//cout << "dist" << endl;
	if(GD_type==1){ 
	  
 	  //cout << "Aqui  test_logM_CovMeans" << endl;

	  mat test_logM_CovMeans = get_emb_LogCov (test_Cov, test_Mean);
	  
	  //cout << "Aqui  train_logM_CovMeans" << endl;
	  mat train_logM_CovMeans = get_emb_LogCov (train_Cov, train_Mean);   
	  
	  //cout <<  norm( test_logM_CovMeans - train_logM_CovMeans, "fro") << endl;
	  dist(k) = norm( test_logM_CovMeans - train_logM_CovMeans, "fro");

	}
	
	else if(GD_type==2){
	  vec subs;
	  mat sum;
	  
	  subs = ( test_Mean - train_Mean );
	  sum = inv( test_Cov ) + inv( train_Cov );
	  
	  //cout << "tmp_dist_a" << endl;
	  tmp_dist_a =sqrt( as_scalar( subs.t()*sum*subs ) );
	  //cout << "tmp_dist_b" << endl;
	  tmp_dist_b = norm( test_logM_Cov - train_logM_Cov, "fro");
	  dist(k) = (1-theta)*tmp_dist_a + theta*tmp_dist_b;
	}
	else{
	  cout << "Options are 1 or 2" << endl;
	  getchar();
	  
	}
	
	++k;
	
      }
    }
  }
  
  
  return dist;
  
}


inline
mat
kth_cv_distNN_svm::get_emb_LogCov(mat cov_i, vec mean_i)
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

