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






//Home
const std::string path = "/media/johanna/HD1T/Datasets/UT-Tower/ut-tower/Video_Frames/";

//WANDA
//const std::string path = "/home/johanna/codes/datasets_codes/KTH/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";







int
main(int argc, char** argv)
{
  mat data;
  data.load("bbox_data.h5", hdf5_binary);
  
  cout << data.n_rows << " & " << data.n_cols << endl;
  vec labels_all_frames = data.col(0);
  cv::Mat frame;
  

    
  
    for (int actID=0; actID<9; ++actID )
    {
      
      uvec q1 = find(labels_all_frames == actID+1); 
      //cout << q1.n_elem << endl;
      mat data_i = data.rows( q1 );  
      //cout << data_i.n_rows << " & " << data_i.n_cols << endl;
      
      vec  videos_action = data_i.col(1);
 
      
      for (int seqID = 0; seqID<12; ++seqID)
      {
        uvec q1 = find(videos_action == seqID+1);  //Frames in action_i sequence_j
        mat data_i_j = data_i.rows( q1 );  
	
        for (int frameID=0; frameID<q1.n_elem; ++frameID)
        {
	  
	  std::stringstream folder_name;
	  folder_name << path << "im" << setw(2) << setfill( '0' ) << actID + 1 << "_" << setw(2) << setfill( '0' ) << seqID + 1 << "_" << setw(3) << setfill( '0' ) << frameID+1 << ".bmp";
	  //cout << folder_name.str() << endl;
	  frame = cv::imread(folder_name.str() );
	  
	  int x = data_i_j(frameID,3);
	  int y = data_i_j(frameID,4);
	  int w = data_i_j(frameID,5);
	  int h = data_i_j(frameID,6);
	  
	  //cout << x << " " <<  y << " " <<  w << " " <<  h << endl;
	  cv::Mat roi(frame, cv::Rect(x-1,y-1,w-1,h-1));
	  
	  //save as features_dim14/scale1-shift0/actionname_seg_##
	  
	  
	  cv::imshow("ori", frame);
	  cv::imshow("bb", roi);
	  cv::waitKey(20);
 	 
        }
        //getchar();
      }
    }
  
  
  
  
  
  return 0;
  
}

// inline
// std::string
// get_name( int actID; int  seqID; int frameID)
// {
//   std::stringstream frame_name;
// 	 frame_name << path << "im"
//   
//   
// }

