%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc

%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); %--> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Wanda

%path  = '/media/johanna/HD1T/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Home


action_seq_names = list_video_sequences;

dim =14;
num_videos = 108; %From the dataset description (108). 
%%%%%%%%%%%%%%%%%%%%   Riemannian Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Kernel LED-Poly. 

%   display('Training svm + Kernel LED-POLY');
%   n=1:dim;
%  
%  for i=1:length(n)
%      in_n = n(i);
%      %sprintf('n = %d ', in_n)
%      disp(in_n);
%      acc = LED_POLY_train(path, action_seq_names, dim, in_n, num_videos);
%      %ACC_train(i,:) = acc;
%  end
% 
%   display('Testing svm + Kernel LED-POLY');
%   n=1:dim;
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

%% Kernel LED-RBF.

display('Training svm + Kernel LED-RBF');
delta = -10:1:9;
ACC_train = zeros(length(delta),num_videos -1);
for i=1:length(delta)
    acc = LED_RBF_train(path, action_seq_names, dim, delta(i), num_videos );
    ACC_train(i,:) = acc;
end

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




%%%%%%%%%%%%%%%%%%%%   Grassmann Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Projection kernel: RBF.

%  display('Training svm + Projection RBF Kernel ');
%  delta = -14:2:20;
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
 
 
% display('Testing svm + Projection RBF Kernel');
% delta = -14:2:20;
% dim = 14;
% p = 1:14;
% test_acc = zeros( length(delta),1 );
% all_p = cell(length(p),1);
% scale = 1;
% shift = 0;
% for j=1:length(p)
%     in_p = p(j);
% for i=1:length(delta )   
%     in_delta = delta(i);
%     X = sprintf('p= %d and delta = %d ', in_p, in_delta);
%     disp(X);
%     acc = ProjectionRBF_test(path, action_seq_names, scale, shift, in_delta, dim, in_p, num_videos);
%     test_acc(i) = acc; 
% end
% all_p{j} = test_acc;
% end
% 
% save_results =strcat( 'projRBF_all_p_delta_performance.mat');
% save(save_results, 'all_p', 'delta', 'p', 'dim', 'scale', 'shift');



%% Projection Poly  Kernel 

% display('Training svm + Projection Poly Kernel ');
% p = 1:14;
% d=1:14;
% 
% for i=1:length(p)
%     for j=1:length(d)
%         in_p = p(i);
%         in_d = d(j);
%         X=sprintf('p = %d, d= %d ', in_p, in_d);
%         disp(X);
%         acc = ProjPOLY_train(path, action_seq_names, dim, in_p, in_d, num_videos);    
%     end
% end



%  display('Testing svm + Projection Poly Kernel');
%  p = 1:14;
%  d=1:14;
%  test_acc = zeros(length(p),length(d));
%  scale = 1;
%  shift = 0;
% 
%  for i=1:length( p )
%      for j=1:length(d)
%         in_p = p(i);
%         in_d = d(j);
%         X=sprintf('p = %d, d= %d ', in_p, in_d);
%         disp(X);
%         acc = ProjPOLY_test(path, action_seq_names, scale, shift, dim, p(i), in_d, num_videos );
%         test_acc(i,j) = acc;
%      end
%  end
% 
% save_results =strcat( 'projPoly_all_p_performance.mat');
% save(save_results, 'test_acc', 'p', 'd', 'dim', 'scale', 'shift');

