%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim =14;

 %%%%%%%%%%%%%%%%%%%%   Riemannian Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Kernel LED-Poly: KTH
% display('Testing svm + Kernel LED-POLY');
% vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
% 
% %n=1:dim;
% best_n = 17;
% all_acc_shifts = zeros( length(vec_shift), 1); 
% scale=1;
%  
%  for i=1:length(vec_shift)
%      shift = vec_shift(i);
%      sprintf('shift = %d ', shift)
%      acc = kth_test_LED_POLY(path,scale, shift, dim, best_n);
%      all_acc_shifts(i) = acc;  
%      all_acc_shifts'
%     
%  end
 
 
display('Testing svm + Kernel LED-POLY. Varying Scale');
vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];
best_n = 17;
all_acc_shifts = zeros( length(vec_scale), 1); 
shift = 0;
 
 for i=1:length(vec_scale)
     scale=vec_scale(i);;
     sprintf('scale = %d ', scale)
     acc = kth_test_LED_POLY(path,scale, shift, dim, best_n);
     all_acc_shifts(i) = acc;  
     all_acc_shifts'
    
 end
