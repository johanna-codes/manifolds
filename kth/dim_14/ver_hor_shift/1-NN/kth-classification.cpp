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

// #include "aux-functions-def.hpp"
// #include "aux-functions-impl.hpp"

// #include "riemannian-metrics-def.hpp"
// #include "riemannian-metrics-impl.hpp"

#include "grassmann-metrics-def.hpp"
#include "grassmann-metrics-impl.hpp"

#include "kth-cross-validation-def.hpp"
#include "kth-cross-validation-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path  = "/home/johanna/codes/codes-git/manifolds/trunk/kth/dim_14/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";



int
main(int argc, char** argv)
{
  
 
    int total_scenes = 1; //Only for Scenario 1.
    int dim = 14; 
    
    field<string> all_people;
    all_people.load(peopleList);
    
    
   

    
    ///Shifting the Images
    
        int best_p = 7;
        int scale_factor = 1;
        
        vec vec_shift;
        vec_shift << -25 << -20 << -15 << -10 << -5 << 0 << 5 << 10 << 15 << 20 << 25 << endr;
        
        vec vec_pm_shifts_hor 		= zeros(vec_shift.n_elem);
        vec vec_logEucl_shifts_hor 	= zeros(vec_shift.n_elem);
	
	vec vec_pm_shifts_ver 		= zeros(vec_shift.n_elem);
        vec vec_logEucl_shifts_ver   	= zeros(vec_shift.n_elem);
        
	
	bool flag_shift;
	
	
        for (int i=0; i< vec_shift.n_elem; ++i)
        {
	  int shift = vec_shift(i);
	  flag_shift = true; //For Horizontal shift
	           
	  cout << "Horizontal Shift: " << shift << endl;
          kth_cv_omp kth_CV_omp_onesegment_hor(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim, flag_shift );
          vec_pm_shifts_hor (i) = kth_CV_omp_onesegment_hor.proj_grass(best_p);
          vec_logEucl_shifts_hor(i) = kth_CV_omp_onesegment_hor.logEucl();
	  
	  flag_shift = false; //For Vertical shift
	  
	  cout << "Vertical Shift: " << shift << endl;
          kth_cv_omp kth_CV_omp_onesegment_ver(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim, flag_shift );
          vec_pm_shifts_ver (i) = kth_CV_omp_onesegment_ver.proj_grass(best_p);
          vec_logEucl_shifts_ver(i) = kth_CV_omp_onesegment_ver.logEucl();
	  
  
          
        }
        cout << "Horizontal Shift" << endl;
        vec_pm_shifts_hor.t().print();
        vec_logEucl_shifts_hor.t().print();
	
	cout << "Vertical Shift" << endl;
        vec_pm_shifts_ver.t().print();
        vec_logEucl_shifts_ver.t().print();
    
    
    

    
    
    
    
    
    
    
    return 0;
    
}
