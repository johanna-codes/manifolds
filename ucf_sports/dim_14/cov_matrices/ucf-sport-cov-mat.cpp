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



//NICTA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ucf_sports/";
const std::string path_dataset = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";





const std::string  actionNames = "actionNames.txt";


int
main(int argc, char** argv)
{
  
//     if(argc < 3 )
//      {
//        cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
//        return -1;
//      }
//      
//      int scale_factor = atoi( argv[1] );
//      int shift = atoi( argv[2] );
  
  
//   vec vec_shift;
//   vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
//   int scale_factor =1;
  
  int shift = 0;
  vec scale_vec;
  scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  
  int dim = 14; 
  
  
  //for (int i=0; i< vec_shift.n_elem; ++i)
  for (int i=0; i<scale_vec.n_elem; ++i)
  {
    float scale_factor = scale_vec(i);
    cout << "Cov for scale_factor " << scale_factor << endl;
    
    //int shift = vec_shift(i);
    //cout << "Cov for shift " << shift << endl;
    cov_mat get_cov_seg(path, path_dataset, actionNames, scale_factor, shift, dim);
    get_cov_seg.calculate_one_per_video(  );
    
  }
  

  return 0;
  
}
