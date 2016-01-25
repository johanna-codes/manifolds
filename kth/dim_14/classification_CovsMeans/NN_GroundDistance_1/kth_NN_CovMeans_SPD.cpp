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


#include "kth-cross-validation-def.hpp"
#include "kth-cross-validation-impl.hpp"


// This codes uses the embedded SPD matrices as per Eq. 4.
//Taken from A Novel Earth Mover’s Distance Methodology for Image Matching with Gaussian Mixture Models


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
  
    int total_scenes = 1; //Only for Scenario 1.
    int dim = 14; 
    
    field<string> all_people;
    all_people.load(peopleList);
    
    int scale_factor = 1;
    int shift = 0;
    float acc_LogEuclidean;
    
    
    //Only one Covariance & Mean per Video
    //kth_cv_omp kth_CV_omp_CovMeans(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
    //acc_LogEuclidean = kth_CV_omp_CovMeans.logEucl(); // Ground Distance 1
    
    
    vec vec_Ng;
    vec_Ng << 2 << 4 << 8 << 16 << endr;
    
    
    for (int i=0; i<vec_Ng.n_elem; ++i)
    {
      int Ng = vec_Ng(i);
      kth_cv_omp kth_CV_omp_CovMeans(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
      acc_LogEuclidean = kth_CV_omp_CovMeans.logEucl_GMM( Ng );//Ground Distance 1 with GMM
      
      cout << "Ng: " << Ng << ". Accuracy: " << acc_LogEuclidean << endl;
      
    }
    

    return 0;
    
}
