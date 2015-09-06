class grass_points
{
public:  
    inline grass_points(const std::string in_path,
			const std::string in_path_dataset,
			const std::string in_actionNames,  
			const float in_scale_factor, 
			const int in_shift,
			const int in_dim,		    
			const int in_p
                  );
    


    inline void calculate_onepervideo(  );

const std::string path;
const std::string path_dataset;
const std::string actionNames;

const float scale_factor;
const int shift;
const int p;
const int dim;

field<std::string> actions;


  private: 
    inline void one_video_one_point( std::string load_feat_video_i, std::string folder_n, std::string action_name );


 
  
};