clear all
clc

%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); %--> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda
path  = '~/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Wanda

%path  = '/media/johanna/HD1T/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Home


action_seq_names = list_video_sequences;

dim =14;
num_videos = 108; %From the dataset description (108).

%% Kernel LED-Poly. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%UT Tower. Shifts
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% display('Testing svm + Kernel LED-POLY');
% 
% scale  = 1;
% best_n = 2;
% vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
% test_acc_shifts = zeros(length(vec_shift),1);
% 
% for i=1:length(vec_shift)
%     shift = vec_shift(i);
%     sprintf('shift = %d ', shift)
%     acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, best_n, num_videos);
%     test_acc_shifts(i) = acc;
% end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%UT Tower. Scales
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

display('Testing svm + Kernel LED-POLY');

shift  = 0;
best_n = 2;
vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];
test_acc = zeros(length(vec_scale),1);

for i=1:length(vec_scale)
    scale = vec_scale(i);
    sprintf('scale = %d ', scale)
    acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, best_n, num_videos);
    vec_scale_scales(i) = acc;
end

%% Kernel proj-rbf. 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%UT Tower. Shifts
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

display('Testing svm + Kernel Proj-RBF');

scale  = 1;
best_p = 4;
best_delta = 10;
vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
test_acc_shifts = zeros(length(vec_shift),1);

for i=1:length(vec_shift)
    shift = vec_shift(i);
    sprintf('shift = %d ', shift)
    acc = ProjectionRBF_test(path, action_seq_names, scale, shift, best_delta, dim, best_p, num_videos);
    test_acc_shifts(i) = acc;
end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%UT Tower. Scales
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

display('Testing svm + Kernel Proj-RBF');

shift  = 0;
best_p = 4;
best_delta = 10;
vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];
test_acc = zeros(length(vec_scale),1);

for i=1:length(vec_scale)
    scale = vec_scale(i);
    sprintf('scale = %d ', scale)
    acc = ProjectionRBF_test(path, action_seq_names, scale, shift, best_delta, best_p, in_p, num_videos);
    vec_scale_scales(i) = acc;
end
