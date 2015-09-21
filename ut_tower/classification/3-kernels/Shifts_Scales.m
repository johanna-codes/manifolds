clear all
clc

%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); %--> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda
path  = '~/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Wanda

%path  = '/media/johanna/HD1T/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Home


action_seq_names = list_video_sequences;

dim =14;
num_videos = 108; %From the dataset description (108).

%% Kernel LED-Poly. UCF Sports

display('Testing svm + Kernel LED-POLY');

scale  = 1;
best_n = 1;
vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
test_acc = zeros(length(vec_shift),1);

for i=1:length(vec_shift)
    shift = vec_shift(i);
    sprintf('shift = %d ', shift)
    acc = LED_POLY_test(path,action_seq_names,scale, shift, dim, best_n, num_videos);
    test_acc(i) = acc;
    test_acc'
end

