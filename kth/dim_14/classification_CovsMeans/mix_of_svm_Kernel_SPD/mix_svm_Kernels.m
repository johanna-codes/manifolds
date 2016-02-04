%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
dbstop error;


%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda

%addpath('/home/johanna-uq/Toolbox/libsvm-320/matlab'); %--> UQ


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;
best_n = 12; 



%Quitar normalizaciones y alpha y beta
%Entrenar cada modelo y luego crear otro vector por cada muestra
%en training y enternar con svm

%Luego probar con Test


%% Kernel LED-Poly

 %get_Kernel_all_runs(path,dim, best_n); 

 %display('Training svm + Kernel LED-POLY + Kernel means');
 %kth_train_1;
 %kth_train_2(path, best_n);
 
 display('Testing svm + Kernel LED-POLY');
 scale = 1;
 shift = 0;
 acc = kth_test(path,scale, shift, best_n)
 
% save_results =strcat( 'LED_POLY_all_n_performance.mat');
% save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


