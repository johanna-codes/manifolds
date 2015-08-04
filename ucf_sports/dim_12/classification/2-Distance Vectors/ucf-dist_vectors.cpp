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

#include "LogEucl-cross-validation-def.hpp"
#include "LogEucl-cross-validation-impl.hpp"

#include "SteinDiv-cross-validation-def.hpp"
#include "SteinDiv-cross-validation-impl.hpp"


#include "GrassPM-cross-validation-def.hpp"
#include "GrassPM-cross-validation-impl.hpp"
 
 
 #include "GrassBC-cross-validation-def.hpp"
 #include "GrassBC-cross-validation-impl.hpp"



const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ucf_sports/";
const std::string path_dataset = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";
const std::string  actionNames = "actionNames.txt";


int
main(int argc, char** argv)
{

  int dim = 14; 
  
 
  
  //********************************************************************************
  // ******************************Training****************************************** 
  //********************************************************************************  
  int scale_factor = 1;
  int shift = 0; 
  //Cross Validation LogEuclidean
  //cout << "Training Log-Euclidean Distance" << endl;
  //cv_dist_vector_LogEucl run_cvLE(path, path_dataset, actionNames, dim);
  //run_cvLE.train(scale_factor, shift);
  //cout << "Testing Log-Euclidean Distance" << endl;
  //run_cvLE.test(scale_factor, shift);
  
  
//   cv_dist_vector_SteinDiv run_cvSD(path, path_dataset, actionNames, dim);
//   cout << "Training Stein Divergence" << endl;
//   run_cvSD.train(scale_factor, shift);
//   cout << "Testing  Stein Divergence" << endl;
//   run_cvSD.test(scale_factor, shift);
  
  
  
  vec vec_bc = zeros(dim);
  vec vec_pm = zeros(dim);
  
  
  for (int p=1; p<=dim; ++p )
     {
       //Cross Validation Grassmann Projection Metric
       cout << "Training for Grassmann PM, p=  " << p << endl;
       cv_dist_vector_GrassPM run_cvPM(path, path_dataset, actionNames, dim, p);
       run_cvPM.train(scale_factor, shift);
       cout << "Testing" << endl;
       vec_pm(p-1) = run_cvPM.test(scale_factor, shift);
       
        //Cross Validation Grassmann Binet-Cauchy Metric
       cout << "Training for Grassmann BC, p=  " << p << endl;
       cv_dist_vector_GrassBC run_cvBC(path, path_dataset, actionNames, dim, p);
       run_cvBC.train(scale_factor, shift);
       cout << "Testing" << endl;
       vec_bc(p-1) = run_cvBC.test(scale_factor, shift);
     }
  
  vec_pm.t().print("Projection Metric");
  vec_bc.t().print("Binet-Cauchy");
  
//********************************************************************************
// ******************************Testing****************************************** 
//********************************************************************************
  
//   int best_p_PM = 8;
//   int best_p_BC = 9;
//   vec vec_shift;
//   vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
//   int scale_factor =1;
//   
//   for (int i=0; i< vec_shift.n_elem; ++i)
//   {
//     
//     int shift = vec_shift(i);
//     cout << "Shift: " << shift << endl;
//     
// //     cout << "Cross Validation for Log-Euclidean Distance" << endl;
// //     kth_cv_svm_LogEucl run_kth_cv_svm_LogEucl(path, actionNames, all_people, total_scenes,  dim);
// //     run_kth_cv_svm_LogEucl.test(scale_factor, shift);
// //     
// //     cout << "Cross Validation for Stein Divergence" << endl;
// //     kth_cv_svm_Stein run_kth_cv_svm_SD(path, actionNames, all_people, total_scenes,  dim);
// //     run_kth_cv_svm_SD.test(scale_factor, shift);
//     
//     cout << "Cross Validation for Grassmann using Projection Metric" << endl;
//     kth_cv_svm_Grass_PM run_kth_cv_svm_PM(path, actionNames, all_people, total_scenes,  dim);
//     run_kth_cv_svm_PM.test(best_p_PM, scale_factor, shift);
//     
//     cout << "Cross Validation for Grassmann using Binet-Cauchy metric" << endl;
//     kth_cv_svm_Grass_BC run_kth_cv_svm_BC(path, actionNames, all_people, total_scenes,  dim);
//     run_kth_cv_svm_BC.test(best_p_BC, scale_factor, shift);
//     cout << "********************************************************************************"<< endl;
//     
//   }
  
  
  
  
  return 0;
  
}
