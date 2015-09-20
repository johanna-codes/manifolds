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
  
  int dim = 14; 
  int shift = 0;
  vec scale_vec;
  scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
  
  
  for (int j=0; j<vec_shift.n_elem; ++j)
  {
    float scale_factor = 1;
    int shift = vec_shift(j);
    cout << "Scale & Shift: " << scale_factor << " & " << shift << endl;
    
    cov_mat get_cov_seg(path, actionNames, scale_factor, shift, dim);
    get_cov_seg.calculate_one_per_video(  );
  }
  
  for (int i=0; i<scale_vec.n_elem; ++i)
  {
    float scale_factor = scale_vec(i);
    int shift = 0;
    cout << "Scale & Shift: " << scale_factor << " & " << shift << endl;
    //float scale_factor = 1;
    //cout << "scale_factor " << scale_factor << endl;
    
    cov_mat get_cov_seg(path, actionNames, scale_factor, shift, dim);
    get_cov_seg.calculate_one_per_video(  );
    
  }
  
  
  
  
  
  
  
  
  
  
  return 0;
  
}
