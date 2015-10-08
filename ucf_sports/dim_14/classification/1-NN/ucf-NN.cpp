#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>
#include <hdf5.h>
#include "omp.h"

 using namespace std;
 using namespace arma;
 
#include "grassmann-metrics-def.hpp"
#include "grassmann-metrics-impl.hpp"
 
#include "cross-validation-NN-def.hpp"
#include "cross-validation-NN-impl.hpp"





//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ucf_sports/";
const std::string path_dataset = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";

const std::string  actionNames = "actionNames.txt";




int
main(int argc, char** argv)
{
  
//    if(argc < 3 )
//   {
//     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
//     return -1;
//   }
//   
//   
//   int scale_factor = atoi( argv[1] );
//   int shift = atoi( argv[2] );
   

  
  //Cross Validation
    //cv_classify_NN CV_onesegment(path, path_dataset, actionNames, scale_factor, shift,  dim);
    //CV_onesegment.logEucl();
    //CV_onesegment.SteinDiv();
  
  
 /// Shifts 
//    vec vec_shift;
//    vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//    int scale_factor = 1;
//    int dim = 14; 
//    int best_p = 2;
//  
//    
//    vec vec_logEucl   	= zeros(vec_shift.n_elem);
//    vec vec_proj_grass	= zeros(vec_shift.n_elem);
//    
//    for (int i=0; i< vec_shift.n_elem; ++i)
//    {
//      
//      int shift = vec_shift(i);
//      cout << "Shift: " << shift << endl;
//      cv_classify_NN CV_onesegment(path, path_dataset, actionNames, scale_factor, shift,  dim);
//      vec_logEucl(i) = CV_onesegment.logEucl();
//      vec_proj_grass(i) = CV_onesegment.proj_grass(best_p);
//     
//    }
//    
//    vec_logEucl.t().print("vec_logEucl");
//    vec_proj_grass.t().print("vec_proj_grass");
  
  
  
  
  ///Scales
    int shift = 0;
    vec scale_vec;
    scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95 << 1 << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
    
    int best_p = 2;
    int dim = 14; 
   
   vec vec_logEucl   = zeros(scale_vec.n_elem);
   vec vec_proj_grass	= zeros(scale_vec.n_elem);
   
   for (int i=0; i< scale_vec.n_elem; ++i)
   {
      float scale_factor = scale_vec(i);
      cout << "scale_factor: " << scale_factor << endl;
      cv_classify_NN CV_onesegment(path, path_dataset, actionNames, scale_factor, shift,  dim);
      vec_logEucl(i) = CV_onesegment.logEucl();
      vec_proj_grass(i) = CV_onesegment.proj_grass(best_p);
   }
 
   
   vec_logEucl.t().print("vec_logEucl");
   vec_proj_grass.t().print("vec_proj_grass");
  
  
  
    
   
/*    
   vec vec_bc = zeros(dim);
   vec vec_pm = zeros(dim);
   for (int p=1; p<= dim; ++p)
   {
     cout << "p= " << p << endl;
     cv_classify_NN CV_onesegment(path, path_dataset, actionNames, scale_factor, shift,  dim);
     vec_pm(p-1) = CV_onesegment.proj_grass(p);
     vec_bc(p-1) = CV_onesegment.BC_grass(p);
   }
   
   vec_pm.t().print("Projection Metric");
   vec_bc.t().print("Binet-Cauchy");*/
//   
//   
  
  
  return 0;
  
}
