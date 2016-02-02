%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
dbstop error;


%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;
best_n = 12; 
vec_alpha = 0:0.1:1;
%vec_alpha = 0;


%see normalise_kernel for options
nor_type = 0; 


%% Kernel LED-Poly

  %get_Kernel_all_runs(path,dim, best_n); 

  display('Training svm + Kernel LED-POLY + Kernel means');
  
   for i=1:length(vec_alpha)
            alpha = vec_alpha(i);
            X=sprintf('n = %5.2f ', alpha);
            disp(X);
            kth_train(alpha, nor_type);
   end
 
  
   display('Testing svm + Kernel LED-POLY');
   
   test_acc = zeros(length(vec_alpha),1);
   scale = 1;
   shift = 0;
  
   for i=1:length(vec_alpha)
          alpha = vec_alpha(i);
          X=sprintf('alpha = %5.2f, ', alpha);
          disp(X);
          acc = kth_test(path,scale, shift, dim, best_n, alpha, nor_type);
          test_acc(i) = acc;
          test_acc'
          %pause
   end
 
% save_results =strcat( 'LED_POLY_all_n_performance.mat');
% save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


