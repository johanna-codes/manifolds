%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim =14;


%% Kernel LED-RBF

% display('Training svm + Kernel LED-RBF');
% delta = -10:1:9;
% ACC_train = zeros(length(delta),25);
% n=1:dim;
% 
% for i=1:length(n)
%     for j=1:length(delta)
%         in_n = n(i);
%         in_delta = delta(j);
%         X=sprintf('n = %d, delta= %d ', in_n, in_delta);
%         disp(X);
%         acc = kth_train_LED_RBF(path, dim, in_n, in_delta);
%     end
% end
% 
% % DO IT: enter in_n in_delta, change load svm_model. change save n_delta  
% display('Testing svm + Kernel LED-RBF');
% delta = -10:1:9;
% test_acc = zeros(length(delta),1);
% scale = 1;
% shift = 0;
% 
%  parfor i=1:length(delta)
%     acc = kth_test_LED_RBF(path,scale, shift, dim, delta(i));
%     test_acc(i) = acc;
%  end
% 
%  save_results =strcat( 'LED_RBF_all_delta_performance.mat');
%  save(save_results, 'test_acc', 'delta', 'dim', 'scale', 'shift');

%% Kernel LED-Poly

display('Training svm + Kernel LED-POLY');
n=1:dim; 


for i=1:length(n)
         in_n = n(i);
         X=sprintf('n = %d ', in_n);
         disp(X);
         acc = kth_train_LED_POLY(path, dim, in_n);
end

 
 display('Testing svm + Kernel LED-POLY');
 %n=1:dim;
 test_acc = zeros(length(n),1);
 scale = 1;
 shift = 0;

 for i=1:length(n)
        in_n = n(i);
        X=sprintf('n = %d, ', in_n);
        disp(X);
        acc = kth_test_LED_POLY(path,scale, shift, dim, in_n);
        test_acc(i) = acc;
        test_acc'
 end

save_results =strcat( 'LED_POLY_all_n_performance.mat');
save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


