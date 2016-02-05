class getVecSPD_GMM
{
public:  
    inline getVecSPD_GMM(const std::string in_path,
		    const std::string in_actionNames,  
		    const float in_scale_factor, 
		    const int in_shift,
		    const int in_scene
                  );
    


    inline void get_SVD( field<string> all_people, int dim );
    //inline void get_gmm_per_video( field<string> all_people, int dim, const int Ng );



const std::string path;
const std::string actionNames;

const float scale_factor;
const int shift;
const int total_scenes;
int dim;

field<std::string> actions;
field<string> all_people;


  private: 
    //inline void gmm_one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act, const int Ng );

};