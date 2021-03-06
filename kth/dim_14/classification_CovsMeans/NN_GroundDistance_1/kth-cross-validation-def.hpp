class kth_cv_omp //KTH Cross validation
{
  
  public:
    
    inline kth_cv_omp(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const float in_scale_factor, 
		const int in_shift,
		const int in_scene, //only for kth
		const int in_dim 
 		);
    inline float logEucl(); //Ground Distance 1
    inline float logEucl_GMM( const int Ng );//Ground Distance 1 with GMM

    
    
    const std::string path;
    const std::string actionNames;
    field<std::string> all_people;
    
    const float scale_factor;
    const int shift;
    const int total_scenes;
    const int dim;
    int p;

    
    field<std::string> actions;
    
    
    
  private:
    inline uword logEucl_one_video( int pe_test, std::string load_sub_path, std::string load_cov);
    inline uword GD_gmm(int pe_test, int act_test, const int Ng);//Ground Distance 1
    inline float dist_te_tr(int pe_test, int pe_train, int act_test, int act_train, const int Ng);    
    inline mat get_emb_LogCov(mat cov_i, vec mean_i);
    

    
    
};