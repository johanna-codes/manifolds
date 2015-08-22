%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc

%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/ucf_sports/';
dataset_path  = '~/codes/datasets_codes/my_ucf_sports_actions/';

%home
%path_dataset = '/media/johanna/HD1T/codes/datasets_codes/my_ucf_sports_actions/';

action_seq_names = list_video_sequences(dataset_path);

dim =14;
num_videos = 150; %From the dataset description (150). Problem with Run-Side_001_dim14
%%%%%%%%%%%%%%%%%%%%   Riemannian Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% LogEuclidean. No hacer todavia
%It was taken from another paper
%dbstop in LogEuclidean_train at 46
%   display('Training svm + Kernel Log-Euclidean Distance');
%   sigma = 0.1:0.1:2;
%   ACC_train = zeros(length(sigma),num_videos -1);
%   for i=1:length(sigma)
%      acc =  LogEuclidean_train(path, action_seq_names, sigma(i), dim, num_videos);
%      ACC_train(i,:) = acc;
%
%   end
%
%    display('Testing svm + Kernel Log-Euclidean Distance');
%    sigma = 0.1:0.1:2;
%    test_acc = zeros(length(sigma),1);
%    scale = 1;
%    shift = 0;
%
%    parfor i=1:length(sigma)
%       acc = LogEuclidean_test(path, action_seq_names, scale, shift, sigma(i), dim, num_videos);
%       test_acc(i) = acc;
%    end
%
%    save_results =strcat( 'LogEucl_all_sigma_performance.mat');
%    save(save_results, 'test_acc', 'sigma', 'dim', 'scale', 'shift');

%% Kernel LED-RBF. Running in NICTA: Done

% display('Training svm + Kernel LED-RBF');
% delta = -10:1:9;
% ACC_train = zeros(length(delta),num_videos -1);
% for i=1:length(delta)
%     acc = LED_RBF_train(path, action_seq_names, dim, delta(i), num_videos );
%     ACC_train(i,:) = acc;
% end

% display('Testing svm + Kernel LED-RBF');
% delta = -10:1:9;
% test_acc = zeros(length(delta),1);
% scale = 1;
% shift = 0;
% 
% for i=1:length(delta)
%     in_delta = delta(i);
%     X = sprintf('delta = %d ', in_delta);
%     disp(delta(i));
%     acc = LED_RBF_test(path,action_seq_names, scale, shift, dim, in_delta, num_videos);
%     test_acc(i) = acc;
% end
% 
% save_results =strcat( 'LED_RBF_all_delta_performance.mat');
% save(save_results, 'test_acc', 'delta', 'dim', 'scale', 'shift');

%% Kernel LED-Poly. DONE

%  display('Training svm + Kernel LED-POLY');
%  %n=1:dim;
%  n=1:20;
% 
%  for i=15:length(n)
%      in_n = n(i);
%      sprintf('n = %d ', in_n)
%      acc = LED_POLY_train(path, action_seq_names, dim, in_n, num_videos);
%      %ACC_train(i,:) = acc;
%  end

%   display('Testing svm + Kernel LED-POLY');
%   %n=1:dim;
%   n=1:20;
%   test_acc = zeros(length(n),1);
%   scale = 1;
%   shift = 0;
% 
%   for i=1:length(n)
%       in_n = n(i);
%       X = sprintf('n = %d ', in_n);
%       acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, in_n, num_videos);
%       test_acc(i) = acc;
%       test_acc'
%   end
% % 
%   save_results =strcat( 'LED_POLY_all_n_performance.mat');
%  save(save_results, 'test_acc', 'n', 'dim', 'scale', 'shift');


%%%%%%%%%%%%%%%%%%%%   Grassmann Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Projection kernel: RBF.
%  Running in NICTA 

%  display('Training svm + Projection RBF Kernel ');
%  delta = -14:1:21;
%  dim = 14;
%  p = 1:14;
%  %p = 12;
%  ACC_train = zeros(length(delta),num_videos -1);
%  all_p = cell(length(p),1);
% 
%  for j=1:length(p)
%      in_p = p(j);
%      %X = sprintf('p = %d ', in_p);
%      %disp(X);
% 
%  for i=1:length(delta)
%      in_delta = delta(i);
%      X = sprintf('p= %d and delta = %d ', in_p, in_delta);
%      disp(X);
%      acc = ProjectionRBF_train(path, action_seq_names, in_delta, dim, in_p, num_videos);
%     %ACC_train(i,:) = acc;
%  end
% 
%  %all_p{j} = ACC_train;
%  end
 
 
display('Testing svm + Projection RBF Kernel');
delta = -14:1:21;
dim = 14;
p = 1:14;
test_acc = zeros( length(delta),1 );
all_p = cell(length(p),1);
scale = 1;
shift = 0;
for j=1:length(p)
    in_p = p(j);
for i=1:length(delta )   
    in_delta = delta(i);
    X = sprintf('p= %d and delta = %d ', in_p, in_delta);
    disp(X);
    acc = ProjectionRBF_test(path, action_seq_names, scale, shift, in_delta, dim, in_p, num_videos);
    test_acc(i) = acc; 
end
all_p{j} = test_acc;
end

save_results =strcat( 'projRBF_all_p_delta_performance.mat');
save(save_results, 'all_p', 'delta', 'p', 'dim', 'scale', 'shift');



%% Projection Kernel: Run in NICTA. DO IT!!!!!!!!!!!

%display('Training svm + Projection Poly Kernel ');
%  dim = 14;
%  p = 1:14;
%  ACC_train = zeros(length(p),25);
%
%
%   parfor i=1:length(p)
%      acc = kth_train_ProjectionPoly(path,  dim, p(i));
%      ACC_train(i,:) = acc;
%   end
%


%  display('Testing svm + Projection Poly Kernel');
%  dim = 14;
%  p = 1:14;
%  test_acc = zeros(length(p),1);
%  scale = 1;
%  shift = 0;
%
%  parfor i=1:length( p )
%     acc = kth_test_ProjectionPoly(path,scale, shift, dim, p(i) );
%     test_acc(i) = acc;
%  end
%
% save_results =strcat( 'projPoly_all_p_performance.mat');
% save(save_results, 'test_acc', 'p', 'dim', 'scale', 'shift');

