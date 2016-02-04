%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
dbstop error;


%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda

%addpath('/home/johanna-uq/Toolbox/libsvm-320/matlab'); %--> UQ


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;
best_n = 12;  % For SPD Kernels

best_delta = 14; % For LS Kernels
best_p = 7; % For LS Kernels



%Quitar normalizaciones y alpha y beta
%Entrenar cada modelo y luego crear otro vector por cada muestra
%en training y enternar con svm

%Luego probar con Test


%% Kernel LED-Poly

 %display('get_SPD_Kernel_all_runs');
 %get_SPD_Kernel_all_runs(path,dim, best_n);  
 
 %display('get_LS_Kernel_all_runs');
 %get_LS_Kernel_all_runs(path,dim, best_delta, best_p);

 %display('Training svm + Kernel LED-POLY + Kernel means');
 %kth_train_1;
  
vec_costs = [0.01 0.1 1 10 100]; 
all_acc = zeros(length(vec_costs),1);
scale = 1;
shift = 0;

 for c = 1:length(vec_costs)
     cost = vec_costs(c);
     X = sprintf('Training Linear SVM with svm_features c = %5.2f ', cost);
     disp(X);  
     kth_train_2(path, best_n, cost, best_delta, best_p, dim);
     acc = kth_test(path,scale, shift, best_n, best_delta, best_p, dim);
     all_acc(c) = acc; 
     all_acc'
 end
 
% save_results =strcat( 'LED_POLY_all_n_performance.mat');
% save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


