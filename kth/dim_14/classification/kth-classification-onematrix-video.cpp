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

// #include "aux-functions-def.hpp"
// #include "aux-functions-impl.hpp"

// #include "riemannian-metrics-def.hpp"
// #include "riemannian-metrics-impl.hpp"

#include "grassmann-metrics-def.hpp"
#include "grassmann-metrics-impl.hpp"

#include "kth-cross-validation-one-def.hpp"
#include "kth-cross-validation-one-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path  = "/home/johanna/codes/codes-git/manifolds/trunk/kth/dim_14/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;





int
main(int argc, char** argv)
{
  
  //    if(argc < 3 )
  //   {
    //     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
    //     return -1;
    //   }
    //   
    //   int scale_factor = atoi( argv[1] );
    //   int shift = atoi( argv[2] );
    
    
    
    int total_scenes = 1; //Only for Scenario 1.
    int dim = 14; 
    
    field<string> all_people;
    all_people.load(peopleList);
    
    
    //Cross Validation
    //    kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
    //    kth_CV_omp_onesegment.logEucl();
    //    kth_CV_omp_onesegment.proj_grass(p);
    //   vec vec_pm = zeros(dim);
    //   for (int p=1; p<= dim; ++p)
    //   {
    //     cout << "p= " << p << endl;
    //     kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
    //     vec_pm(p-1) = kth_CV_omp_onesegment.proj_grass(p);
    //   }
    //   
    //   vec_pm.t().print("Projection Metric");
    
    
    /// Extra experiments 
//     int scale_factor = 1;
//     int shift = 0;
//     kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//     kth_CV_omp_onesegment.SteinDiv();
//     kth_CV_omp_onesegment.logEucl();
//     
//     vec vec_bc = zeros(dim); 
//     vec vec_pm = zeros(dim);
//     for (int p=1; p<= dim; ++p)
//     {
//          cout << "p= " << p << endl;
//          kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//          vec_bc(p-1) = kth_CV_omp_onesegment.BC_grass(p);
// 	 vec_pm(p-1) = kth_CV_omp_onesegment.proj_grass(p);
//       
//     }
//        
//        vec_bc.t().print("Binet-Cauchy");
//        vec_pm.t().print("Projection Metric");
    
    
    
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
    
    ///Shifting the Images
    
//        int best_p = 7;
//        int scale_factor = 1;
//        
//        vec vec_shift;
//        vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//        
//        vec vec_pm_shifts = zeros(vec_shift.n_elem);
//        vec vec_logEucl_shifts   = zeros(vec_shift.n_elem);
//        
//        for (int i=0; i< vec_shift.n_elem; ++i)
//        {
//            
//          int shift = vec_shift(i);
//          cout << "Shift: " << shift << endl;
//          kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//          vec_pm_shifts(i) = kth_CV_omp_onesegment.proj_grass(best_p);
//          vec_logEucl_shifts(i) = kth_CV_omp_onesegment.logEucl();
//          
//        }
//        
//        vec_pm_shifts.t().print();
//        vec_logEucl_shifts.t().print();
    
    
    ///Varying the  Scales
    int best_p = 7;
    int shift = 0;
    vec scale_vec;
     scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95 << 1 << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
     
     
     vec vec_logEucl   = zeros(scale_vec.n_elem);
     vec vec_pm_shifts = zeros(scale_vec.n_elem);
     
     for (int i=0; i< scale_vec.n_elem; ++i)
     {
       
       float scale_factor = scale_vec(i);
       cout << "scale_factor: " << scale_factor << endl;
       kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
       vec_logEucl(i) = kth_CV_omp_onesegment.logEucl();
       vec_pm_shifts(i) = kth_CV_omp_onesegment.proj_grass(best_p);
       
     }
     
     vec_logEucl.t().print();
     vec_pm_shifts.t().print();
    
    
    
    
    
    
    
    return 0;
    
}
