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




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/study-paper/trunk/features/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;



int
main(int argc, char** argv)
{
  
  
  int shift = 0;
  vec scale_vec;
  scale_vec << 0.75 <<  0.80 << 0.85 << 0.90 << 0.95  << 1.05 << 1.10 << 1.15 << 1.20 << 1.25 << endr;
  
  int total_scenes = 1; //Only for Scenario 1.
  int segment_length = 20;
  int dim = 14; 
  
  
  for (int i=0; i<scale_vec.n_elem; ++i)
  {
    float scale_factor = scale_vec(i);
    cout << "Cov for scale_factor " << scale_factor << endl;
    
    field<string> all_people;
    all_people.load(peopleList);
    
    cov_mat_kth get_cov_seg(path, actionNames, scale_factor, shift, total_scenes, segment_length);
    //get_cov_seg.calculate( all_people, dim );
    get_cov_seg.calculate_one_per_video( all_people, dim );
    
  }
  

  return 0;
  
}






/*
int
main(int argc, char** argv)
{
  
  //  if(argc < 3 )
  //   {
  //     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
  //     return -1;
  //   }
  //   
  //   int scale_factor = atoi( argv[1] );
  //   int shift = atoi( argv[2] );
  
  
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
  int scale_factor =1;
  
  int total_scenes = 1; //Only for Scenario 1.
  int segment_length = 20;
  int dim = 14; 
  
  
  for (int i=0; i< vec_shift.n_elem; ++i)
  {
    
    int shift = vec_shift(i);
    cout << "Cov for " << shift << endl;
    field<string> all_people;
    all_people.load(peopleList);
    
    cov_mat_kth get_cov_seg(path, actionNames, scale_factor, shift, total_scenes, segment_length);
    //get_cov_seg.calculate( all_people, dim );
    get_cov_seg.calculate_one_per_video( all_people, dim );
    
  }
  

  return 0;
  
}*/
