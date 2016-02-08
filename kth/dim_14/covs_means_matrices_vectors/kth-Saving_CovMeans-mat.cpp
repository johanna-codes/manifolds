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

#include "Saving_CovMeans-def.hpp"
#include "Saving_CovMeans-impl.hpp"


// This codes save the means and covariance matrices for one video.
//Taken from A Novel Earth Mover’s Distance Methodology for Image Matching with Gaussian Mixture Models
// WIth Cov and Means. Eq(8) is going to be used.

//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/kth/dim_14/features/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;

int
main(int argc, char** argv)
{
  
     
   int total_scenes = 1; //Only for Scenario 1.
   int dim = 14;  
   
   vec vec_Ng;
   vec_Ng << 4 << 8 << 16 << endr;
   //int Ng =2;
   
   field<string> all_people;
   all_people.load(peopleList);
   
  
  

   
   //cout << arma_version::as_string() << endl;
   
   
//    // Ideal (Clean) Data
//   float scale_factor = 1;
//   int shift = 0;
//    for (int i = 0; i< vec_Ng.n_elem; ++i)
//    {
//      int Ng = vec_Ng(i);
//      cout << Ng << endl;
//      
      CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes);
      get_CovMeans.calculate_one_per_video( all_people, dim );
//      get_CovMeans.get_gmm_per_video( all_people, dim, Ng );
//    
//    }
//   
   
   
   //  
   
   ///Varying the Scale
   
//    int shift = 0;
//    vec scale_vec;
//    scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
//    
//    for (int i = 0; i< scale_vec.n_elem; ++i)
//    {
//      float scale_factor = scale_vec(i);
//      cout << "scale_factor= " << scale_factor << endl;
//      
//      CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes);
//      get_CovMeans.calculate_one_per_video( all_people, dim );
//    
//    }
   
   
   
  
  
///Shifting Videos

  
//   float scale_factor = 1;
//   vec vec_shift;
//   //vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//   vec_shift << 0;

//   
  
  // Horizontal shift
//   bool flag_shift = true; //For Horizontal shift
//   for (int i=0; i<vec_shift.n_elem; ++i)
//   {
//     int shift = vec_shift(i);
//     cout << "Cov for Hor_shift " << shift << endl;

//     
//     CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes);
//     get_CovMeans.calculate_one_per_video( all_people, dim, flag_shift );
//     
//   }
  
  
  // Vertical shift
//   bool  flag_shift = false; //For Vertical shift
//  
//   
//   for (int i=0; i<vec_shift.n_elem; ++i)
//   {
//     int shift = vec_shift(i);
//     cout << "Cov for Ver_shift " << shift << endl;
//     

//     
//     CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes);
//     get_CovMeans.calculate_one_per_video( all_people, dim, flag_shift );
//     
//   }
  
  
  
  
  
  
  
  ///Shifting horizontal- and vertically at the same time. Shifting Videos
  
//   float scale_factor = 1;
//   vec vec_shift;
//   vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//   
//   
//   
//   
//   for (int i=0; i<vec_shift.n_elem; ++i)
//   {
//     int shift = vec_shift(i);
//     cout << "Cov for shift " << shift << endl;
//     

//     
//     CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes, segment_length);
//     get_CovMeans.calculate_one_per_video( all_people, dim );
//     
//   }
//   
  
  ///Varying the Scale
  //   int shift = 0;
  //   vec scale_vec;
  //   scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  //   
  //   
  //   
  //   for (int i=0; i<scale_vec.n_elem; ++i)
  //   {
    //     float scale_factor = scale_vec(i);
    //     cout << "Cov for scale_factor " << scale_factor << endl;
    //     
    //     
    //     CovMeans_mat_kth get_CovMeans(path, actionNames, scale_factor, shift, total_scenes, segment_length);
    //     get_CovMeans.calculate_one_per_video( all_people, dim );
    //     
    //   }
    
    
    return 0;
    
}







