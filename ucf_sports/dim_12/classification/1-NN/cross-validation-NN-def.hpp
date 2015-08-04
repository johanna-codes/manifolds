class cv_classify_NN //KTH Cross validation
{
  
  public:
    
    inline cv_classify_NN(const std::string in_path,
			  const std::string in_path_dataset,
			  const std::string in_actionNames,  
			  const int in_scale_factor, 
			  const int in_shift,
			  const int in_dim 
			 );

    inline float proj_grass(int p);
    inline float BC_grass(int p);

    
    
    const std::string path;
    const std::string path_dataset;
    const std::string actionNames;

    
    const int scale_factor;
    const int shift;
    const int dim;
    int p;

    
    field<std::string> actions;
    
    
    
  private:
     inline uword ProjectionMetric_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp);
    inline uword BinetCauchyMetric_one_video(field <std::string> action_seq_names, int test_i, std::string load_sub_path, std::string load_Gnp);
    
    
};