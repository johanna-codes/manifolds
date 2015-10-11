%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim =14;

 %%%%%%%%%%%%%%%%%%%%   SPD Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Kernel LED-Poly: KTH
vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];

%n=1:dim;
best_n = 12;
all_acc_shifts_hor = zeros( length(vec_shift), 1); 
all_acc_shifts_ver = zeros( length(vec_shift), 1); 
scale=1;
 
 for i=1:length(vec_shift)
     shift = vec_shift(i);
     sprintf('Horizontal shift = %d ', shift)
     flag_shift = true; %For horizontal Shift
     acc = kth_test_LED_POLY_hor_ver(path, scale, shift, dim, best_n, flag_shift);
     all_acc_shifts_hor(i) = acc;  
     
     sprintf('Vertical shift = %d ', shift)
     flag_shift = false;
     acc = kth_test_LED_POLY_hor_ver(path, scale, shift, dim, best_n, flag_shift);
     all_acc_shifts_ver(i) = acc;  

    
 end
 
 
 %%%%%%%%%%%%%%%%%%%%   Grasmann Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Kernel proj-rbf: KTH
%  display('Testing svm + Kernel Proj-RBF. Shifting Videos');
%  vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
%  
%  all_acc_shifts = zeros( length(vec_shift), 1); 
%  scale = 1;
%  best_delta = 14;
%  best_p = 7;
%   for i=1:length(vec_shift)
%       shift = vec_shift(i);
%       sprintf('shift = %d ', shift)
%       acc = kth_test_ProjectionRBF(path,scale, shift, best_delta, dim, best_p);
%       all_acc_shifts(i) = acc;  
%       all_acc_shifts'
%      
%   end
%  
%  
