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

#include "cov-mat-def.hpp"
#include "cov-mat-impl.hpp"



//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ut_tower/";

//Casita
//const std::string path =  "/media/johanna/HD1T/codes/codes-git/manifolds/trunk/ut_tower/";
//creo que no necesito el path_dataset
//cambiar path por el de ut-tower. ver en wanda

const std::string  actionNames = "actionNames.txt";


int
main(int argc, char** argv)
{
  
  
  int shift = 0;
  float scale_factor = 1;
  
  int dim = 14; 
  cov_mat get_cov_seg(path, actionNames, scale_factor, shift, dim);
  get_cov_seg.calculate_one_per_video(  );
    
    

  return 0;
  
}
