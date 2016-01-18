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


#include "kth_SVM_CV_def.hpp"
#include "kth_SVM_CV_impl.hpp"



//Taken from A Novel Earth Mover’s Distance Methodology for Image Matching with Gaussian Mixture Models
// Using Eq. (8) as NN distance

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
    int Ng = 2;
    
    field<string> all_people;
    all_people.load(peopleList);
    
    int scale_factor = 1;
    int shift = 0;
    float acc_GD; //Ground Distance
    // 1. for GD_1 + SVM. 
    // 2 for GD_2 + SVM
    // 3. for GD_1 + GMM +SVM.
    int GD_type = 1; 
    
    kth_cv_distNN_svm NN_SVM_GD_2(path, actionNames, all_people,  total_scenes,  dim, GD_type );
    NN_SVM_GD_2.train( scale_factor, shift );
    NN_SVM_GD_2.test( scale_factor, shift );
    
    
    


    
    return 0;
    
}
