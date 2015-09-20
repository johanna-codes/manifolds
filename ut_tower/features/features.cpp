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



//Casita
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/ut_tower/";

//WANDA
const std::string path = "/home/johanna/codes/datasets_codes/ut_tower/";




const std::string  actionNames = "actionNames.txt";


int
main(int argc, char** argv)
{
  
  
  
  
  int shift = 0;
  int dim = 14;
  
  vec scale_vec;
  scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
  
  
  
  for (int j=0; j<vec_shift.n_elem; ++j)
  {
    float scale_factor = 1;
    int shift = vec_shift(j);
    cout << "Scale & Shift: " << scale_factor << " & " << shift << endl;
    //float scale_factor = 1;
    //cout << "scale_factor " << scale_factor << endl;
    
    opt_feat opt_feat_ut(path, actionNames, scale_factor, shift, dim);
    opt_feat_ut.features_all_videos();
  }
  
  for (int i=0; i<scale_vec.n_elem; ++i)
  {
    float scale_factor = scale_vec(i);
    int shift = 0;
    cout << "Scale & Shift: " << scale_factor << " & " << shift << endl;
    //float scale_factor = 1;
    //cout << "scale_factor " << scale_factor << endl;
    
    opt_feat opt_feat_ut(path, actionNames, scale_factor, shift, dim);
    opt_feat_ut.features_all_videos();
    
  }
  
  
  
  return 0;
  
}




//mkdir scale1-shift0 scale1-shift10 scale1-shift-10 scale1-shift15 scale1-shift-15 scale1-shift20 scale1-shift-20 scale1-shift25 scale1-shift-25 scale1-shift5 scale1-shift-5

//mkdir scale0.75-shift0 scale0.8-shift0 scale0.85-shift0 scale0.9-shift0 scale0.95-shift0 scale1.05-shift0 scale1.1-shift0 scale1.15-shift0 scale1.2-shift0 scale1.25-shift0


// int
// main(int argc, char** argv)
// {
//   
//   if(argc < 3 )
//   {
//     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
//     return -1;
//     
//   }
//   
//   
//   int scale_factor = atoi( argv[1] );
//   int shift = atoi( argv[2] );
//   int dim = 14;
//   
// 
//   
//   opt_feat opt_feat_ucf(path, actionNames, scale_factor, shift, dim);
//   opt_feat_ucf.features_all_videos();
//   //opt_feat_kth.features_all_videos( all_people );
//   
//  
//   
//   return 0;
//   
// }


