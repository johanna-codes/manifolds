%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim =14;

%%%%%%%%%%%%%%%%%%%%   Riemannian Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% LogEuclidean
%It was taken from another paper

% display('Training svm + Kernel Log-Euclidean Distance');
% sigma = 1:0.1:2;
% ACC_train = zeros(length(sigma),25);
% parfor i=1:length(sigma)
%    acc = kth_train_LogEuclidean(path, sigma(i), dim);
%    ACC_train(i,:) = acc;
%
% end
%
%  display('Testing svm + Kernel Log-Euclidean Distance');
%  sigma = 0.1:0.1:2;
%  test_acc = zeros(length(sigma),1);
%  scale = 1;
%  shift = 0;
%
%  parfor i=1:length(sigma)
%     acc = kth_test_LogEuclidean(path,scale, shift, sigma(i), dim);
%     test_acc(i) = acc;
%  end
%
%  save_results =strcat( 'LogEucl_all_sigma_performance.mat');
%  save(save_results, 'test_acc', 'sigma', 'dim', 'scale', 'shift');

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

% display('Training svm + Kernel LED-POLY');
% n=1:dim; % No funciono para ningun n>21
% 
% 
% for i=1:length(n)
%          in_n = n(i);
%          X=sprintf('n = %d ', in_n);
%          disp(X);
%          acc = kth_train_LED_POLY(path, dim, in_n);
% end
% 
%  
%  display('Testing svm + Kernel LED-POLY');
%  %n=1:dim;
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


%%%%%%%%%%%%%%%%%%%%   Grassmann Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Projection kernel: RBF
 display('Training svm + Projection RBF Kernel ');
 delta = -14:2:20;
 p = 1:14;
 ACC_train = zeros(length(delta),25);
 all_p = cell(length(p),1);

 for j=1:length(p)
     in_p = p(j);
     X = sprintf('p = %d ', in_p);
     disp(X);

 parfor i=1:length(delta)
    acc = kth_train_ProjectionRBF(path, delta(i), dim, in_p);
    ACC_train(i,:) = acc;
 end

 all_p{j} = ACC_train;
 end


display('Testing svm + Projection RBF Kernel');
delta = -14:2:20;
dim = 14;
p = 1:14;
test_acc = zeros( length(delta),1 );
all_p = cell(length(p),1);
scale = 1;
shift = 0;
for j=1:length(p)
    in_p = p(j)
parfor i=1:length(delta )
   acc = kth_test_ProjectionRBF(path,scale, shift, delta (i), dim, in_p);
   test_acc(i) = acc;
end
all_p{j} = test_acc;
end

save_results =strcat( './svm_results_projRBF/projRBF_all_p_delta_performance.mat');
save(save_results, 'all_p', 'delta', 'p', 'dim', 'scale', 'shift');



%% Projection Kernel: Poly
% display('Training svm + Projection Poly Kernel ');
% dim = 14;
% p = 1:14;
% d=1:14;
% %ACC_train = zeros(length(p),25);
% 
% 
% 
% for i=1:length(p)
%     for j=1:length(d)
%         in_p = p(i);
%         in_d = d(j);
%         X=sprintf('p = %d, d= %d ', in_p, in_d);
%         disp(X);
%         acc = kth_train_ProjectionPoly(path,  dim, in_p, in_d);
%         %  ACC_train(i,:) = acc;
%     end
% end



% display('Testing svm + Projection Poly Kernel');
% dim = 14;
% p = 1:14;
% d=1:14;
% test_acc = zeros(length(p),length(d));
% scale = 1;
% shift = 0;
% 
% for i=1:length( p )
%     for j=1:length(d)
%         in_p = p(i);
%         in_d = d(j);
%         X=sprintf('p = %d, d= %d ', in_p, in_d);
%         disp(X);
%         acc = kth_test_ProjectionPoly(path,scale, shift, dim, in_p, in_d );
%         test_acc(i,j) = acc;
%     end
% end
% save_results =strcat( 'projPoly_all_p_performance.mat');
% save(save_results, 'test_acc', 'p', 'dim', 'scale', 'shift');

