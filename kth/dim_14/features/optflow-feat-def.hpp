class opt_feat
{
public:  
  inline opt_feat(const std::string in_path,
		  const std::string in_actionNames,  
		  const int in_col, 
		  const int in_row,
		  const float in_scale_factor, 
		  const int in_shift,
		  const int in_scene,
		  const int dim,
		  const bool in_flag_shift
  );
  
  inline void features_all_videos( field<string>all_people);
  
  
  
  
  const std::string path;
  const std::string actionNames;
  
  const int col;
  const int row;
  const float scale_factor;
  const int shift;
  const int total_scene;
  const int dim; 
  const bool flag_shift;
  
//   vector <vec> features_video_i; //All vectors per video_i are stored here
//   vector <int> labels_video_i;
  
  
  field<std::string> actions;
  
  struct Struct_feat_lab{
    vector <vec> features_video_i; //All vectors per video_i are stored here
    vector <int> labels_video_i;

};

  
private:

  
  
  inline void feature_video( std::string one_video, Struct_feat_lab &my_Struct_feat_lab );
  inline cv::Mat Shift_Image( cv::Mat src_in, int num_pixels_x, int num_pixels_y);
  inline cv::Mat Scale_Image( cv::Mat src_in, float scale_factor);
  inline cv::Mat Shift_Image_Horizontal( cv::Mat src_in, int num_pixels_x );
{
  
  
  
  
};