class grass_points
{
public:  
    inline grass_points(const std::string in_path,
		    const std::string in_actionNames,  
		    const float in_scale_factor, 
		    const int in_shift,
		    const int in_scene,
		    const int segment_length,
		    const int in_p,
		    const bool in_flag_shift
                  );
    


    inline void calculate( field<string> all_people, int dim );
    inline void calculate_onepervideo( field<string> all_people, int dim  );

const std::string path;
const std::string actionNames;

const float scale_factor;
const int shift;
const int total_scenes;
const int segment_length;
const int p;
int dim;
const bool flag_shift;

field<std::string> actions;
field<string> all_people;


  private: 
    inline void one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act );
    inline void one_video_one_point( std::string load_feat_video_i, int sc, int pe, int act );


 
  
};