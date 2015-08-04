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

#include "grass-def.hpp"
#include "grass-impl.hpp"




//NICTA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/ucf_sports/";
const std::string path_dataset = "/home/johanna/codes/datasets_codes/my_ucf_sports_actions/";


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
  
  
//   vec vec_shift;
//   vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
//   int scale_factor =1;
  
  int dim = 14; 
  
  //for (int i=0; i< vec_shift.n_elem; ++i)
  {
    for (int p=1; p<= dim; ++p)
    {
      
      //int shift = vec_shift(i);
      cout << "Gp for shift " << shift << " &  p= " << p << endl;
      

      
      grass_points get_gp(path, path_dataset, actionNames, scale_factor, shift, dim, p);
      get_gp.calculate_onepervideo( );
    }
  }
  
  return 0;
  
}
