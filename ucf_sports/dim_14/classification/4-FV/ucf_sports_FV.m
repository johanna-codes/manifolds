%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');
clear all
close all
clc

dim = 14;
Ncent = 256;

Ng = int2str(Ncent);

path  = '~/codes/codes-git/manifolds/trunk/ucf_sports/';
dataset_path  = '~/codes/datasets_codes/my_ucf_sports_actions/';




dim_FV = 2*dim*Ncent;
vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];

action_seq_names = list_video_sequences(dataset_path);
num_videos = 150; %From the dataset description (150). 

%% Get FV. Run Just once. Run all shifts when features are ready

%  scale_factor = 1;
%  for i=1:length(vec_shift)
%      show_you = strcat('Getting FVs for ', int2str( vec_shift(i) ) );
%      disp(show_you);
%      FV_ucf_sports_all_videos( path, Ncent, dim, scale_factor, vec_shift(i), num_videos, action_seq_names );
%  end

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
%     action_name = action_seq_names(video_ts,1);
%     folder_n    = action_seq_names(video_ts,2);
%     
%     
%     if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
%         %ts = [action_name,'_',folder_n];
%         %disp(ts);
%         for video_tr=1: num_videos
%             if (video_tr~=video_ts)
%                 action_name_tr = action_seq_names(video_tr,1);
%                 folder_n_tr    = action_seq_names(video_tr,2);
%                 act_tr  =  str2double( action_seq_names(video_tr,3) );
%                 if (~(strcmp(action_name_tr, 'Run-Side') && strcmp(folder_n_tr,'001')))
%                     name_load_FV = strcat( load_sub_path, '/FV_', action_name, '_', folder_n, '_Ng', Ng, '.txt');
%                     one_FV = load( char( name_load_FV ) );
%                     X_train(:,k) = one_FV;
%                     labels_train(k) = act_tr;
%                     k=k+1;
%                 end
%             end
%         end
%         
%         data_train = X_train';
%         model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
%         save_svm_model = strcat( './svm_models/run_video', int2str(video_ts), '.mat')
%         save(save_svm_model, 'model');
%     end
% end





%% Testing TODO: Run all shifts when features are ready
 
scale_factor = 1;
shift = 0;
acc = 0;
real_labels = zeros(num_videos - 1);%Problem with Run-Side_001_dim14
est_labels  = zeros(num_videos - 1);
  

load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-shift',  int2str(shift));

j=1;
  for video_ts= 1: num_videos
      action_name = action_seq_names(video_ts,1);
      folder_n    = action_seq_names(video_ts,2);
      act_ts  =  str2double( action_seq_names(video_tr,3) );
      
      if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
          load_svm_model =  strcat( './svm_models/run_video', int2str(video_ts), '.mat');
          load(load_svm_model); %loading model and X_train
      
          
          name_load_FV = strcat( load_sub_path, '/FV_', action_name, '_', folder_n, '_Ng', Ng, '.txt');
          one_FV = load( char( name_load_FV ) );
          X_test(:,j) = one_FV;
          [predicted_label, accuracy, prob_estimates] = svmpredict([act_ts], X_train', model, ['-b 1']);
          
          real_labels(j) = act_ts;
          est_labels(j) = predicted_label;
          j=j+1;
          
          if predicted_label == act_ts
              acc = acc+1;
          end

      end
      
      save_labels = strcat('./svm_results/scale', int2str(scale_factor), '-shift', int2str(shift),'.mat' );
      save(save_labels, 'est_labels', 'real_labels');
  end
  acc = acc*100/(num_videos - 1);

