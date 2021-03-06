class getVecSPD_GMM
{
public:  
    inline getVecSPD_GMM(const std::string in_path,
		    const std::string in_actionNames,  
		    const float in_scale_factor, 
		    const int in_shift,
		    const int in_scene,
		    const int in_dim
                  );
    


    inline void get_one_vec_video( field<string> all_people);
    inline void get_GMM( field<string> in_all_people, int N_cent);

    
    inline void get_vecs_video( field<string> in_all_people, int num_SPD);
    inline void get_GMM_all_vecs( field<string> all_people, int N_cent, int num_SPD );




const std::string path;
const std::string actionNames;

const float scale_factor;
const int shift;
const int total_scenes;
const int dim;


field<std::string> actions;
field<string> all_people;


  private: 
    inline vec get_vec( mat cov, int dim_spdvec );
    //inline void gmm_one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act, const int Ng );

};