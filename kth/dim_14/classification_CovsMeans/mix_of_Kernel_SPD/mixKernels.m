%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;




%% Kernel LED-Poly

display('Training svm + Kernel LED-POLY');
best_n = 12;
 
vec_alpha = 0.1:0.1:1;

for i=1:length(vec_alpha)
         alpha = vec_alpha(i);
         X=sprintf('n = %5.2f ', alpha);
         disp(X);
         acc = kth_train(path, dim, best_n, aplha);
end

 
%  display('Testing svm + Kernel LED-POLY');
%  n=1:dim;
%  test_acc = zeros(length(n),1);
%  scale = 1;
%  shift = 0;
% 
%  for i=1:length(n)
%         in_n = n(i);
%         X=sprintf('n = %d, ', in_n);
%         disp(X);
%         acc = kth_test_LED_POLY(path,scale, shift, dim, in_n);
%         test_acc(i) = acc;
%         test_acc'
%  end
% 
% save_results =strcat( 'LED_POLY_all_n_performance.mat');
% save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


