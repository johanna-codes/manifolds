class cov_mat
{
public:  
    inline cov_mat(const std::string in_path,
		       const std::string in_actionNames,  
		       const int in_scale_factor, 
		       const int in_shift,
		       const int in_dim
                  );
    


    inline void calculate_one_per_video( );


const std::string path;
const std::string actionNames;

const int scale_factor;
const int shift;
const int dim;

field<std::string> actions;


  private: 
    inline void one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i,  std::string folder_n, std::string action_name );


 
  
};