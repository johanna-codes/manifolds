class kth_cv_distNN_svm //KTH Cross validation
{
  
  public:
    
    inline kth_cv_distNN_svm(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scene, //only for kth
		const int in_dim,
		const int in_GD_type
 		);   
    
    inline void train(int tr_scale, int tr_shift);
    inline float test(int ts_scale, int ts_shift);
    
    
    inline void train_gmm(int tr_scale, int tr_shift, int in_Ng);
    inline float test_gmm(int ts_scale, int ts_shift, int in_Ng);

    
    
    const std::string path;
    const std::string actionNames;
    field<std::string> all_people;
    

    const int total_scenes;
    const int dim;
    const int GD_type;
    
    int n_actions;
    int n_peo;
    
    int scale;
    int shift;
    int Ng;
  

    
    field<std::string> actions;
    
    
    
  private:
    
    inline void distances();
    inline void svm_train();
    inline vec dist_one_video(int pe_test, std::string load_sub_path, std::string load_Covs, std::string load_logMCovs, std::string load_Means);
    
    
    inline void svm_train_gmm();
    inline void distances_gmm();
    inline vec dist_one_video_gmm( int pe_test, int act_test);
    inline float dist_te_tr_GD1(int pe_test, int pe_train, int act_test, int act_train);

    inline mat get_emb_LogCov(mat cov_i, vec mean_i);
    
       
};