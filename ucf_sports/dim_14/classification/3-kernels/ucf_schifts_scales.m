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

%% Kernel LED-Poly. UCF Sports. Scales

display('Testing svm + Kernel LED-POLY');

shift  = 0;
best_n = 1;
vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];
test_acc = zeros(length(vec_scale),1);

for i=1:length(vec_scale)
    scale = vec_scale(i);
    sprintf('scale = %d ', scale)
    acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, best_n, num_videos);
    test_acc(i) = acc;
    test_acc'
end

%% Kernel LED-Poly. UCF Sports. Shifts

% display('Testing svm + Kernel LED-POLY');
% 
% scale  = 1;
% best_n = 1;
% vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
% test_acc = zeros(length(vec_shift),1);
% 
% for i=1:length(vec_shift)
%     shift = vec_shift(i);
%     sprintf('shift = %d ', shift)
%     acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, best_n, num_videos);
%     test_acc(i) = acc;
%     test_acc'
% end



