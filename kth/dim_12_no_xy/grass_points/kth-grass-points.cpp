#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>
#include <hdf5.h>


using namespace std;
using namespace arma;

#include "grass-def.hpp"
#include "grass-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/manifolds/trunk/kth/dim_12_no_xy/features/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth


int
main(int argc, char** argv)
{
  
  
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
  int scale_factor =1;
  
  int total_scenes = 1; //Try only with scenario 1
  int segment_length = 20; // Not Used
  //int p   = 12; //To obtain the optimise sub-space of order p
  int dim = 12; 
  int best_p = 7;
  
  for (int i=0; i< vec_shift.n_elem; ++i)
  {
    int shift = vec_shift(i);
    
    cout << "Gp for shift " << shift << " &  p= " << best_p << endl;
    
    
    field<string> all_people;
    all_people.load(peopleList);
    
    grass_points get_gp(path, actionNames, scale_factor, shift, total_scenes, segment_length, best_p);
    //get_gp.calculate( all_people, dim );
    get_gp.calculate_onepervideo( all_people, dim );
  }
  
  
  return 0;
  
}



/*
 * int
 * main(int argc, char** argv)
 * {
 *  
 *  //    if(argc < 3 )
 *  //   {
 *  //     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
 *  //     return -1;
 *  //   }
 *  //   
 *  //   
 *  //   int scale_factor = atoi( argv[1] );
 *  //   int shift = atoi( argv[2] );
 *  
 *  
 *  //vec vec_shift;
 *  //vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
 *  int scale_factor =1;
 *  
 *  int total_scenes = 1; //Try only with scenario 1
 *  int segment_length = 20; // Not Used
 *  //int p   = 12; //To obtain the optimise sub-space of order p
 *  int dim = 12; 
 *  
 *  //for (int i=0; i< vec_shift.n_elem; ++i)
 *  {
 *    int shift = 0;
 *    for (int p=1; p<= dim; ++p)
 *    {
 *      
 *      
 *      cout << "Gp for shift " << shift << " &  p= " << p << endl;
 *      
 *      
 *      field<string> all_people;
 *      all_people.load(peopleList);
 *      
 *      grass_points get_gp(path, actionNames, scale_factor, shift, total_scenes, segment_length, p);
 *      //get_gp.calculate( all_people, dim );
 *      get_gp.calculate_onepervideo( all_people, dim );
 }
 }
 
 return 0;
 
 }*/
