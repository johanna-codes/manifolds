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
  
   if(argc < 3 )
  {
    cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
    return -1;
  }
  
  
  int scale_factor = atoi( argv[1] );
  int shift = atoi( argv[2] );
  int dim = 14; 
  

  
  //Cross Validation
    cv_classify_NN CV_onesegment(path, path_dataset, actionNames, scale_factor, shift,  dim);
    CV_onesegment.logEucl();
//     CV_onesegment.SteinDiv();
//     CV_onesegment.proj_grass(p);

   
    
//   vec vec_bc = zeros(dim);
//   vec vec_pm = zeros(dim);
//   for (int p=1; p<= dim; ++p)
//   {
//     cout << "p= " << p << endl;
//     kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//     vec_pm(p-1) = kth_CV_omp_onesegment.proj_grass(p);
//     vec_bc(p-1) = kth_CV_omp_onesegment.BC_grass(p);
//   }
//   
//   vec_pm.t().print("Projection Metric");
//   vec_bc.t().print("Binet-Cauchy");
//   
//   
  
  
  return 0;
  
}
