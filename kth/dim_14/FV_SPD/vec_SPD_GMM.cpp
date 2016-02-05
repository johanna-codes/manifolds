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
   
   vec vec_Ng;
   vec_Ng << 4 << 8 << 16 << endr;
   //int Ng =2;
   
   field<string> all_people;
   all_people.load(peopleList);
   
   float scale = 1;
   int shift = 0;
   
   getVecSPD_GMM get_SVD( path, actionNames,  scale, shift, total_scenes );
   get_SVD.get_SVD(all_people);
  
  

    
    return 0;
    
}







