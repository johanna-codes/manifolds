inline
opt_feat::opt_feat(const std::string in_path,
		   const std::string in_actionNames,  
		   const float in_scale_factor, 
		   const int in_shift,
		   const int in_dim
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), dim(in_dim)
{
  
  actions.load( actionNames );  
  //dim = 14; 
  //dim = 12; //Action Recognition from Video Using feature Covariance Matrices
}

inline
void
opt_feat::features_all_videos( ) 
{
  mat data;
  data.load("bbox_data.h5", hdf5_binary);
  
  //cout << data.n_rows << " & " << data.n_cols << endl;
  vec labels_all_frames = data.col(0);
  
  
  for (int actID=0; actID<9; ++actID )
  {
    
    uvec q1 = find(labels_all_frames == actID+1); 
    //cout << q1.n_elem << endl;
    mat data_i = data.rows( q1 );  
    //cout << data_i.n_rows << " & " << data_i.n_cols << endl;
    
    vec  videos_action = data_i.col(1);
    
    
    for (int seqID = 0; seqID<12; ++seqID)
    {
      uvec q1 = find(videos_action == seqID+1);  //Frames in action_i sequence_j
      mat data_i_j = data_i.rows( q1 );
      int n_frames = q1.n_elem;
      
      
      Struct_feat_lab my_Struct_feat_lab;
      feature_video( data_i_j, actID, seqID, n_frames, my_Struct_feat_lab );
      mat mat_features_video_i;
      //vec lab_video_i;
      //getchar();
      
      if (my_Struct_feat_lab.features_video_i.size()>0)
      {
	mat_features_video_i.zeros( dim,my_Struct_feat_lab.features_video_i.size() );
	//lab_video_i.zeros( my_Struct_feat_lab.features_video_i.size() );
	
	for (uword i = 0; i < my_Struct_feat_lab.features_video_i.size(); ++i)
	{
	  mat_features_video_i.col(i) = my_Struct_feat_lab.features_video_i.at(i)/norm(my_Struct_feat_lab.features_video_i.at(i),2);
	  //lab_video_i(i) = my_Struct_feat_lab.labels_video_i.at(i);
	  
	}
    }
    else
    {
      mat_features_video_i.zeros(dim,0);
    }
    
    
    my_Struct_feat_lab.features_video_i.clear();
    my_Struct_feat_lab.labels_video_i.clear();
    
    
    std::stringstream save_feat_video_i ;
    save_feat_video_i << "./features_dim" << dim <<  "/scale" << scale_factor << "-shift"<< shift ;
    save_feat_video_i <<  "/" << actions(actID)  << "_seq" <<  seqID << ".h5";
    mat_features_video_i.save( save_feat_video_i, hdf5_binary );
      

    }
  }
}
  
  
  


// //****************** Feature Extraction**************************************
// //***************************************************************************


inline 
void
opt_feat::feature_video(mat data_i_j, int actID, int seqID, int n_frames, Struct_feat_lab &my_Struct_feat_lab)
//( std::string folder_path, Struct_feat_lab &my_Struct_feat_lab )
{
  cv::Mat prevgray, gray, flow, cflow, orig, prevflow;
  cv::Mat ixMat, iyMat, ixxMat, iyyMat;
  cv::Mat flow_xy[2], mag, ang;
 
  string text;
  
  for (int frameID=0; frameID<n_frames; ++frameID)
  {
    std::stringstream folder_name;
    folder_name << path << "im" << setw(2) << setfill( '0' ) << actID + 1 << "_" << setw(2) << setfill( '0' ) << seqID + 1 << "_" << setw(3) << setfill( '0' ) << frameID+1 << ".bmp";
    //cout << folder_name.str() << endl;
    orig = cv::imread(folder_name.str() );
    
    int x = data_i_j(frameID,3);
    int y = data_i_j(frameID,4);
    int w = data_i_j(frameID,5);
    int h = data_i_j(frameID,6);
    
    cv::Mat frame(orig, cv::Rect(x-1,y-1,w-1,h-1));
    
    cv::resize( frame, frame, cvSize(32, 32) );
    
    
    
     int row = frame.rows;
     int col = frame.cols;
     //cout << row << " & " << col << endl;
     
     
     int new_row = row;
     int new_col = col;
     
     if (shift!=0)
     {
       int shif_x = floor(col*shift/100);
       int shif_y = floor(row*shift/100);
       frame = Shift_Image( frame, shif_x, shif_y);
       
     }
     
     if (scale_factor!=1)
     {
       frame =  Scale_Image( frame,  scale_factor);
     }
     
     
     cv::cvtColor(frame, gray, CV_BGR2GRAY);
     
     if( prevgray.data )
     {
       cv::calcOpticalFlowFarneback(prevgray, 
 				   gray, 
 				   flow, 
 				   0.5, //pyr_scale
 				   3,   //levels
 				   9,   //winsize
 				   1,   //iterations
 				   5,   //poly_n
 				   1.1, //poly_sigma
 				   0);  //flags
       
       
       //optical flow
       cv::split(flow, flow_xy);
       cv::cartToPolar(flow_xy[0], flow_xy[1], mag, ang, true);
       
       cv::Sobel(gray, ixMat, CV_32F, 1, 0, 1);
       cv::Sobel(gray, iyMat, CV_32F, 0, 1, 1);
       cv::Sobel(gray, ixxMat, CV_32F, 2, 0, 1);
       cv::Sobel(gray, iyyMat, CV_32F, 0, 2, 1);
       
       float  ux = 0, uy = 0, vx = 0,  vy = 0;
       float u, v;
       
       if( prevflow.data )
       {
 	//cout << new_col << " " << new_row << endl;
 	
 	for (int x = 0 ; x < new_col ; ++x ){
 	  for (int y = 0 ; y < new_row ; ++y ) {
 	    
 	    vec features_one_pixel(dim);
 	    u = flow.at<cv::Vec2f>(y, x)[0];
 	    v = flow.at<cv::Vec2f>(y, x)[1];
 	    
 	    //cout << "x= " << x << " - y= " << y << endl;
 	    // x grad
 	    //cout << " x y grad" << endl;
 	    float ix = ixMat.at<float>(y, x);
 	    //cout << " y grad" << endl;
 	    float iy = iyMat.at<float>(y, x);
 	    
 	    // grad direction &  grad magnitude. (Edges)
 	    //cout << "grad direction &  grad magnitude" << endl;
 	    float gd = std::atan2(std::abs(iy), std::abs(ix));
 	    float gm = std::sqrt(ix * ix + iy * iy);
 	    
 	    // x second grad
 	    //cout << "x y  second grad " << endl;
 	    float ixx = ixxMat.at<float>(y, x);
 	    // y second grad
 	    float iyy = iyyMat.at<float>(y, x);
 	    
 	    //du/dt
 	    float ut = u - prevflow.at<cv::Vec2f>(y, x)[0];
 	    // dv/dt
 	    float vt = v - prevflow.at<cv::Vec2f>(y, x)[1];
 	    
 	    
 	    
 	    if (x>0 && y>0 )
 	    {
 	      ux = u - flow.at<cv::Vec2f>(y, x - 1)[0];
 	      uy = u - flow.at<cv::Vec2f>(y - 1, x)[0];
 	      vx = v - flow.at<cv::Vec2f>(y, x - 1)[1];
 	      vy = v - flow.at<cv::Vec2f>(y - 1, x)[1];
 	    }
 	    
 	    
 	    float Div = (ux + vy);
 	    float Vor = (vx - uy);
 	    //Adding more features
 	    mat G (2,2);
 	    mat S;
 	    float gd_opflow = ang.at<float>(y,x);
 	    float mg_opflow = mag.at<float>(y,x);
 	    //Gradient Tensor
 	    G   << ux << uy << endr
 	    << vx << vy << endr;
 	    
 	    //Rate of Stein Tensor  
 	    S = 0.5*(G + G.t());
 	    
 	    float tr_G = trace(G);
 	    float tr_G2 = trace( square(G) );
 	    float tr_S = trace(S);
 	    float tr_S2 = trace(square(S));
 	    
 	    //Tensor Invariants  of the optical flow
 	    float Gten = 0.5*( tr_G*tr_G - tr_G2 );
 	    float Sten = 0.5*( tr_S*tr_S - tr_S2 ); 
 
 	    if (dim ==14)
 	    {
 	      
 	      features_one_pixel  << x << y << abs(ix) << abs(iy) << abs(ixx) 
 	      << abs(iyy) << gm << gd <<  u << v << abs(ut) 
 	      << abs(vt) << (ux + vy)  << (vx - uy);
 	    }
 	    
 	    if (!is_finite( features_one_pixel ) )
 	    {
 	      cout << "It's not FINITE... continue???" << endl;
 	      getchar(); 
 	    }
 	    // Plotting Moving pixels
 	    //cout << " " << gm;
 	    
 	    double is_zero = accu(abs(features_one_pixel));
 	    
 	    if (gm>40 && is_finite( features_one_pixel ) && is_zero!=0 ) // Empirically set to 40
 	    {   
 	      
 	      frame.at<cv::Vec3b>(y,x)[0] = 0;
 	      frame.at<cv::Vec3b>(y,x)[1] = 0;
 	      frame.at<cv::Vec3b>(y,x)[2] = 255;
 	      my_Struct_feat_lab.features_video_i.push_back(features_one_pixel);
 	      //my_Struct_feat_lab.labels_video_i.push_back( frameID );
 	    }
 	  }
 	}
       }
       
       if(cv::waitKey(30)>=0)
 	break;
       
     }
     
     std::swap(prevgray, gray);
     std::swap(prevflow, flow);
    
    //cv::imshow("color", frame);
    //cv::waitKey(10);
  }
}

inline 
cv::Mat
opt_feat::Shift_Image( cv::Mat src_in, int num_pixels_x, int num_pixels_y)
{
  
  cv::Mat img_out;
  cv::Mat rot_mat = (cv::Mat_<double>(2,3) << 1, 0, num_pixels_x, 0, 1, num_pixels_y);
  warpAffine( src_in, img_out, rot_mat, src_in.size() );
  
  if (num_pixels_x>0) //Move right
  {   
    cv::Mat col = src_in.col(0);
    cv::Mat row = src_in.row(0);
    
    for (int i=0; i<abs(num_pixels_x); ++i)
    {
      col.col(0).copyTo(img_out.col(i));
      
    }
    
    for (int i=0; i<abs(num_pixels_y); ++i)
    {
      row.row(0).copyTo(img_out.row(i));
      //src_in.copyTo(img_out,crop);
    }
  }
  
  if (num_pixels_x<0) //Move right
  {   
    int w = src_in.size().width;
    int h = src_in.size().height;
    cv::Mat col = src_in.col(w-1);
    cv::Mat row = src_in.row(h-1);
    
    for (int i=w-abs(num_pixels_x) ; i<w; ++i)
    {
      col.col(0).copyTo(img_out.col(i));
      //row.row(0).copyTo(img_out.row(i));
    } 
    
    for (int i=h-abs(num_pixels_y) ; i<h; ++i)
    {
      //col.col(0).copyTo(img_out.col(i));
      row.row(0).copyTo(img_out.row(i));
    }
  }
  
  return img_out;
}

inline 
cv::Mat
opt_feat::Scale_Image( cv::Mat src_in, float scale_factor)
{
  cv::Mat scaled_image, dst, tmp;
  tmp = src_in;
  
  //cout << "** Original Size " << tmp.cols << " & " << tmp.rows << endl;
  
  int n_cols =  int(tmp.cols*scale_factor);
  int n_rows =  int(tmp.rows*scale_factor);
  
  
  
  
  
  if (scale_factor>1)
  {
    cv::resize(tmp, dst, cv::Size(n_cols, n_rows ), CV_INTER_CUBIC );
    //cout << "** New Size " << dst.cols << " & " << dst.rows << endl;
    int mid_col_ori = tmp.cols/2;
    int mid_row_ori = tmp.rows/2;
    
    int mid_col_new = dst.cols/2;
    int mid_row_new = dst.rows/2;
    
    int rec_col = mid_col_new - mid_col_ori;
    int rec_row = mid_row_new - mid_row_ori;
    
    cv::Rect roi(rec_col, rec_row, tmp.cols, tmp.rows);
    scaled_image = dst(roi);
    
    
  }
  else if (scale_factor==1)
  {
    scaled_image = tmp;
    
  }
  else
  {
    cv::resize(tmp, dst, cv::Size(n_cols, n_rows ) , CV_INTER_AREA);
    //cout << "** New Size " << dst.cols << " & " << dst.rows << endl;
    
    int x = (tmp.cols - dst.cols)/2;
    int y = (tmp.rows - dst.rows)/2;
    cv::Mat black_border(tmp.size(),CV_8UC3, cv::Scalar(0,0,0)); // all white image  
    dst.copyTo( black_border( cv::Rect( x, y, dst.cols, dst.rows) )  );    
    scaled_image = black_border;
    
    //Adding Vertical Pixels
    //At the beginning
    cv::Mat col = tmp.col(0);
    
    for (int i=0; i<x ; ++i)
    {
      col.col(0).copyTo(scaled_image.col(i));
      
    }
    
    //At the end
    col = tmp.col(tmp.cols-1);
    for (int i=x+dst.cols; i<tmp.cols ; ++i)
    {
      col.col(0).copyTo(scaled_image.col(i));      
    }
    
    //Adding horizontal pixels
    cv::Mat row = tmp.row(0);
    
    for (int i=0; i<y; ++i)
    {
      row.row(0).copyTo(scaled_image.row(i));
      //src_in.copyTo(img_out,crop);
    }
    row = tmp.row(tmp.rows-1);
    
    for (int i=y+dst.rows; i<tmp.rows; ++i)
    {
      row.row(0).copyTo(scaled_image.row(i));
      
      
    }
  }
  
  return scaled_image;
  
}
