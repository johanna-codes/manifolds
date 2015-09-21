%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');
clear all
close all
clc

dim = 14;
Ncent = 256;
Ng = int2str(Ncent);
path  = '~/codes/codes-git/manifolds/trunk/ut_tower';

dim_FV = 2*dim*Ncent;
action_seq_names = list_video_sequences;
num_videos = 108; %From the dataset description (150).

%% Get FV. Run Just once. Run all shifts & do Scales
% scale_factor = 1;
% shift = 0;
% FV_ut_tower_all_videos( path, Ncent, dim, scale_factor, shift, num_videos, action_seq_names );

% for i=1:length(vec_shift)
%     show_you = strcat('Getting FVs for ', int2str( vec_shift(i) ) );
%     disp(show_you);
%     FV_ut_tower_all_videos( path, Ncent, dim, scale_factor, vec_shift(i), num_videos, action_seq_names );
% end

%% Training
% scale_factor = 1;
% shift = 0;
% n_test =  num_videos - 2; %Problem with Run-Side_001_dim14
% 
% load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-shift',  int2str(shift));
% 
% for video_ts= 1: num_videos
%     X_train = zeros(dim_FV,n_test);
%     labels_train = zeros(n_test,1);
%     k =1;
%     
%     for video_tr=1: num_videos
%         if (video_tr~=video_ts)
%             
%             action_name_tr = action_seq_names(video_tr,1);
%             act_tr  =  str2double( action_seq_names(video_tr,2) );
%             seqID_tr    = action_seq_names(video_tr,3);
%             
%             name_load_FV = strcat( load_sub_path, '/FV_', action_name_tr, '_seq', seqID_tr, '_Ng', Ng, '.txt');
%             one_FV = load( char( name_load_FV ) );
%             X_train(:,k) = one_FV;
%             labels_train(k) = act_tr;
%             k=k+1;
%             
%         end
%     end
%     
%     data_train = X_train';
%     model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
%     [predicted_label, accuracy, prob_estimates] = svmpredict(labels_train, data_train, model, ['-b 1']);
%     save_svm_model = strcat( './svm_models/run_video', int2str(video_ts), '.mat');
%     save(save_svm_model, 'model');
% end





%% Testing 

vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];
%vec_shift = [0];
all_acc_shifts = zeros( length(vec_shift), 1);

for i=1:length(vec_shift)
    
    scale_factor = 1;
    shift = vec_shift(i);
    acc = 0;
    real_labels = zeros(num_videos,1);%Problem with Run-Side_001_dim14
    est_labels  = zeros(num_videos,1);
    
    
    load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-shift',  int2str(shift));
    show_you = strcat('Testing shift ', int2str(shift) );
    disp(show_you);
    j=1;
    for video_ts= 1: num_videos
        
        action_name_ts = action_seq_names(video_ts,1);
        act_ts  =  str2double( action_seq_names(video_ts,2) );
        seqID_ts    = action_seq_names(video_ts,3);
        
        
        load_svm_model =  strcat( './svm_models/run_video', int2str(video_ts), '.mat');
        load(load_svm_model); %loading model and X_train
        
        
        name_load_FV = strcat( load_sub_path, '/FV_', action_name_ts, '_seq', seqID_ts, '_Ng', Ng, '.txt');
        one_FV = load( char( name_load_FV ) );
        X_test = one_FV;
        [predicted_label, accuracy, prob_estimates] = svmpredict([act_ts], X_test', model, ['-b 1']);
        real_labels(j) = act_ts;
        est_labels(j) = predicted_label;
        j=j+1;
        
        if predicted_label == act_ts
            acc = acc+1;
        end
        
        
        save_labels = strcat('./svm_results/scale', int2str(scale_factor), '-shift', int2str(shift),'.mat' );
        save(save_labels, 'est_labels', 'real_labels');
    end
    acc = acc*100/(num_videos - 1);
    all_acc_shifts(i) = acc;
end

[vec_shift' all_acc_shifts]

