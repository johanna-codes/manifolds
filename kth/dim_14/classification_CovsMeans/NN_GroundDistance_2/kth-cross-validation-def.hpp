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
    
    inline float Ground_Distance();
    inline float Ground_Distance_GMM(const int Ng);

    
    
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
    inline uword GD_one_video( int pe_test, std::string load_sub_path, std::string load_cov, std::string load_logMCovs, std::string load_Means );
    

    
    
};