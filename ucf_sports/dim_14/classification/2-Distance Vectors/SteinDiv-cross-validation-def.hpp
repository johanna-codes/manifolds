class cv_dist_vector_SteinDiv //KTH Cross validation
{
  
  public:
    
    inline cv_dist_vector_SteinDiv(const std::string in_path,
			  const std::string in_path_dataset,
			  const std::string in_actionNames,  
			  const int in_dim 
 		);   
    
    inline void train(int tr_scale, int tr_shift);
    inline void  test(int ts_scale, int ts_shift);

    const std::string path;
    const std::string path_dataset;
    const std::string actionNames;
    const int dim;

    
    field<std::string> actions;
    
    
    
  private:
    
    inline void distances(int scale_factor, int shift);
    inline void svm_train();
    inline vec dist_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_cov);
    
       
};