#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>

using namespace std;
using namespace arma;

#include "optflow-feat-def.hpp"
#include "optflow-feat-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/datasets_codes/KTH/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
int ori_col = 160;
int ori_row = 120;





int
main(int argc, char** argv)
{
  
  
//    if(argc < 3 )
//   {
//     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
//     return -1;
//   }
//   float scale_factor = atof( argv[1] );
//   int shift = atoi( argv[2] );



//     if(argc < 3 )
//    {
//      //if true: Horizontal Shift
//      cout << "usage: " << argv[0] << " shift_factor, true/false " << endl;
//      return -1;
//    }
   
   //int shift = atoi( argv[1] );
   //std::stringstream ss(argv[2]);
   //bool flag_shift;
   
   //ss >> std::boolalpha >> flag_shift;
   //cout << flag_shift << endl;
   
  
  
  //Visualise the shift:
   int shift = 15;
   int shif_x;
   int shif_y;
   float scale_factor = 1;
   int total_scene = 1; //Hacer solo para scenario 1
   int dim = 14;
   
   bool flag_shift = true; //For Horizontal shift //Pare este ejemplo no importa
//   // bool flag_shift = false; //For Vertical shift
      
      
  cv::Mat horImage, verImage, pos_hor_verImage, neg_hor_verImage, magImage, shiImage;
  opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim, flag_shift);
  
  cv::Mat src_in;
  
  cv::VideoCapture capVideo("/home/johanna/codes/datasets_codes/KTH/boxing/person01_boxing_d1_uncomp.avi");
  int n_frames = capVideo.get(CV_CAP_PROP_FRAME_COUNT);
  
  
  
  for(int fr=0; fr<n_frames; fr++){
    
   shift = 15;
   shif_x = floor(ori_col*shift/100);
   shif_y = floor(ori_row*shift/100);
    
    
    //For Shifts:
    bool bSuccess = capVideo.read(src_in); // read a new frame from video
    //horImage = opt_feat_kth.Shift_Image_Horizontal( src_in,  shif_x );
    //verImage = opt_feat_kth.Shift_Image_Vertical( src_in,  shif_y );
    pos_hor_verImage = opt_feat_kth.Shift_Image( src_in,  shif_x,  shif_y);
    
    shift = -15;
    shif_x = floor(ori_col*shift/100);
    shif_y = floor(ori_row*shift/100);
   
    neg_hor_verImage = opt_feat_kth.Shift_Image( src_in,  shif_x,  shif_y);
    
    
    
    //For Scale
    scale_factor = 0.80;
    shiImage = opt_feat_kth.Scale_Image( src_in, scale_factor);

    
    scale_factor = 1.20;
    magImage = opt_feat_kth.Scale_Image( src_in, scale_factor);

    
      
    //Showing Images and Saving
    cv::imshow("Original", src_in);
    //cv::imshow("Hor-Shift", horImage);
    //cv::imshow("Ver-Shift", verImage);
    cv::imshow("Positive both_Shifts", pos_hor_verImage);
    cv::imshow("Negative both_Shifts", neg_hor_verImage);
       
    cv::imshow("Shrinking", shiImage);
    cv::imshow("Magnificient", magImage);
    
    
    
    cv::imwrite("original.png", src_in);
    cv::imwrite("hor_ver_shifts_positive.png", pos_hor_verImage);
    cv::imwrite("hor_ver_shifts_negative.png", neg_hor_verImage); 
    cv::imwrite("shiImage.png", shiImage);
    cv::imwrite("magImage.png", magImage);
    
    cv::waitKey();
    
    
   
    
    
   
    
   
  }
 

  
  
  

///Shifting Videos
//   float scale_factor = 1;
//   vec vec_shift;
//   vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//   
//   int total_scene = 1; //Hacer solo para scenario 1
//   int dim = 14;
//   
//   field<string> all_people;
//   all_people.load(peopleList);
//   bool flag_shift = true; //For Horizontal shift
//   // bool flag_shift = false; //For Vertical shift
// 
//    
//   
//   for (int i=0; i<vec_shift.n_elem; ++i)
//   {
//     int shift = vec_shift(i);
//     cout << "Horizontal shift " << shift << endl;
// 
//     opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim, flag_shift);
//     opt_feat_kth.features_all_videos( all_people );
//   
//   }
//   
//   
//    flag_shift = false; //For Vertical shift
//    
//    for (int i=0; i<vec_shift.n_elem; ++i)
//    {
//      int shift = vec_shift(i);
//      cout << "Vertical shift " << shift << endl;
//      opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim, flag_shift);
//      opt_feat_kth.features_all_videos( all_people );
//   }
  
 
 ///Varying the Scale
//   int shift = 0;
//   vec scale_vec;
//   scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
//   
//   int total_scene = 1; //Hacer solo para scenario 1
//   int dim = 14;
//   
//   field<string> all_people;
//   all_people.load(peopleList);
//   
//   
//   for (int i=0; i<scale_vec.n_elem; ++i)
//   {
//     float scale_factor = scale_vec(i);
//     cout << "scale_factor " << scale_factor << endl;
// 
//     opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim);
//     opt_feat_kth.features_all_videos( all_people );
//   
//   }
  
  
  
  
  return 0;
  
}

 //Scale-factors:
 //0.75, 0.80, 0.85, 0.90, 0.95, 1.0, 1.05, 1.10, 1.15, 1.20, 1.25
 
//mkdir scale0.75-shift0 scale0.8-shift0 scale0.85-shift0 scale0.9-shift0 scale0.95-shift0 scale1.05-shift0 scale1.1-shift0 scale1.15-shift0 scale1.2-shift0 scale1.25-shift0


//Shifts:
//mkdir scale1-horshift-25 scale1-horshift-20 scale1-horshift-15 scale1-horshift-10 scale1-horshift-5 scale1-horshift0 scale1-horshift5 scale1-horshift10 scale1-horshift15 scale1-horshift20 scale1-horshift25
//mkdir scale1-vershift-25 scale1-vershift-20 scale1-vershift-15 scale1-vershift-10 scale1-vershift-5 scale1-vershift0 scale1-vershift5 scale1-vershift10 scale1-vershift15 scale1-vershift20 scale1-vershift25


//both
//mkdir scale1-shift-25 scale1-shift-20 scale1-shift-15 scale1-shift-10 scale1-shift-5 scale1-shift0 scale1-shift5 scale1-shift10 scale1-shift15 scale1-shift20 scale1-shift25