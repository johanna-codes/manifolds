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

#include "optflow-feat-def.hpp"
#include "optflow-feat-impl.hpp"




//NICTA
const std::string path = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";

//WANDA
//const std::string path = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";



const std::string  actionNames = "actionNames.txt";





int
main(int argc, char** argv)
{
  
  if(argc < 3 )
  {
    cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
    return -1;
    
  }
  
  
  int scale_factor = atoi( argv[1] );
  int shift = atoi( argv[2] );
  int dim = 14;
  

  
  opt_feat opt_feat_ucf(path, actionNames, scale_factor, shift, dim);
  opt_feat_ucf.features_all_videos();
  //opt_feat_kth.features_all_videos( all_people );
  
 
  
  return 0;
  
}


