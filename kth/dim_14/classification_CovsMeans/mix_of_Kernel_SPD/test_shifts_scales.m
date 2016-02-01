%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14 + 1;


display('Testing svm + Kernel LED-POLY');
 
vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];
all_acc_scales = zeros( length(vec_scale), 1); 


 %scale = 1;
 shift = 0;
 
 best_n = 12;

 for i=1:length(vec_scale)
     scale = vec_scale(i);
     
     X=sprintf('scale = %5.2f, ', scale);
     disp(X);
     acc = kth_test_LED_POLY(path,scale, shift, dim, best_n);
     all_acc_scales(i) = acc;
     all_acc_scales'
 end

