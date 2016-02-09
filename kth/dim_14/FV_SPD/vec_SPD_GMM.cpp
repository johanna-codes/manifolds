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

#include "getVecSPDGMM-def.hpp"
#include "getVecSPDGMM-impl.hpp"


// This codes save the means and covariance matrices for one video.
//Taken from A Novel Earth Mover’s Distance Methodology for Image Matching with Gaussian Mixture Models
// WIth Cov and Means. Eq(8) is going to be used.

//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/kth/dim_14/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;

int
main(int argc, char** argv)
{
  
     
   int total_scenes = 1; //Only for Scenario 1.
   int dim =14;
   
   
   vec vec_Ng; // For GMM
   vec_Ng << 2 << 4 << 8 << 16 << endr;
   
   

   
   //int Ng =2;
   
   field<string> all_people;
   all_people.load(peopleList);
   
   float scale = 1;
   int shift = 0;
   
   
   //For One Covariance per Video
//   for (int i=0; i<vec_Ng.n_elem; ++i)
//    {
//      int Ng = vec_Ng(i);
//      getVecSPD_GMM get_vecs( path, actionNames,  scale, shift, total_scenes, dim );
//      get_vecs.get_one_vec_video( all_people );
//      get_vecs.get_GMM( all_people, Ng );
//      
//   }
  
  
  
  
   //Several Covs per videos
   vec vec_num_SPD; // For # of SPD matrices per video
   vec_num_SPD << 2 << 4 << 8 << 16 << endr;
  
    for (int i=0; i<vec_num_SPD.n_elem; ++i)
    {
      int num_SPD = vec_num_SPD(i);
      getVecSPD_GMM get_vecs( path, actionNames,  scale, shift, total_scenes, dim );
      get_vecs.get_vecs_video( all_people, num_SPD);
    
    }
  
  for (int i=0; i<vec_Ng.n_elem; ++i)
  {
    int Ng = vec_Ng(i);
    for (int i=0; i<vec_num_SPD.n_elem; ++i)
   {
     int num_SPD = vec_num_SPD(i);
     getVecSPD_GMM get_vecs( path, actionNames,  scale, shift, total_scenes, dim );
     get_vecs.get_GMM_all_vecs( all_people, Ng, num_SPD );
   
   }
   
  }
  

    
    return 0;
    
}







