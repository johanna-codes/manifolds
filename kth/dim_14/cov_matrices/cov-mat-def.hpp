class cov_mat_kth
{
public:  
    inline cov_mat_kth(const std::string in_path,
		    const std::string in_actionNames,  
		    const float in_scale_factor, 
		    const int in_shift,
		    const int in_scene,
		    const int segment_length,
		    const bool in_flag_shift
                  );
    


    inline void calculate( field<string> all_people, int dim );
    inline void calculate_one_per_video( field<string> all_people, int dim );


const std::string path;
const std::string actionNames;

const float scale_factor;
const int shift;
const int total_scenes;
const int segment_length;
int dim;
const bool flag_shift;

field<std::string> actions;
field<string> all_people;


  private: 
    inline void one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act );
    inline void one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act );


 
  
};