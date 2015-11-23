%********************************************************************
% Sparse Coding and Dictionary Learning for Symmetric
%Positive Definite Matrices: A Kernel Approach
%ECCV 2012
%********************************************************************

clear all
clc
dbstop error;

addpath('/home/johanna/codes/Mehrtash/harandi_eccv_2012_matlab/');% --> Wanda


%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); %--> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda
path  = '~/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Wanda
%path  = '/media/johanna/HD1T/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Home


action_seq_names = list_video_sequences;

dim =14;
num_videos = 108; %From the dataset description (108).
n_test =  num_videos - 1; 


scale_factor = 1;
shift = 0;


%prompt = 'SR_Lambda? ';
%SR_Lambda_input = input(prompt);
SR_Lambda_input = 0.5;

prompt = 'Beta ';
Beta = input(prompt);
%Beta = 1e1;

%vec_Beta = 1:1:10;
%all_means = zeros(length(vec_Beta),1);



%for b=1:length(vec_Beta)
%Beta = vec_Beta(b)
load_sub_path =strcat(path, 'cov_matrices/one-cov-mat/scale', int2str(scale_factor), '-shift', int2str(shift) );

acc = [];

for video_ts= 1: num_videos
    X_train = zeros(dim,dim,n_test);
    labels_train = zeros(1,n_test);
    k =1;
    
    % Joining Training Data
        for video_tr=1: num_videos
            if (video_tr~=video_ts)
                action_name_tr = action_seq_names(video_tr,1);
                act_tr  =  str2double( action_seq_names(video_tr,2) );
                seqID_tr    = action_seq_names(video_tr,3);
                
                    %tr = [action_name_tr,'_',folder_n_tr];
                    %disp(tr);
                    name_load_cov_tr = strcat( load_sub_path, '/cov_', action_name_tr, '_seq', seqID_tr, '.h5');
                    hinfo_tr = hdf5info( char(name_load_cov_tr) );
                    one_video_tr = hdf5read(hinfo_tr.GroupHierarchy.Datasets(1));
                    %disp(one_video);
                    X_train(:,:,k) = one_video_tr;
                    labels_train(k) = act_tr;
                    k=k+1;
            end
        end
        
        %Testing Set only has one video
        action_name_ts = action_seq_names(video_ts,1);
        act_ts  =  str2double( action_seq_names(video_ts,2) );
        seqID_ts    = action_seq_names(video_ts,3);
        
         name_load_cov_ts = strcat( load_sub_path, '/cov_', action_name_ts, '_seq', seqID_ts, '.h5');
         hinfo_ts = hdf5info( char(name_load_cov_ts) );
         one_video_ts = hdf5read(hinfo_ts.GroupHierarchy.Datasets(1));
        
        labels_test = act_ts;
        X_test(:,:,1) = one_video_ts;
        
        
        
        
        TrainSet(1).X = X_train;
        TrainSet(1).y = labels_train;
        
        TestSet(1).X = X_test;
        TestSet(1).y = labels_test;
        
        %% As per example in Code
        
        CRR = RSR(TrainSet,TestSet,Beta,SR_Lambda_input);
        %fprintf('Correct recognition accuracy with a labeled dictionary : %.1f%%.\n',100*CRR)
        
        acc = [acc CRR];
        
       
end
SR_Lambda_input
Beta
mean(acc)*100
%acc
%size(acc)
%all_means(b) =mean(acc)*100;
%end

