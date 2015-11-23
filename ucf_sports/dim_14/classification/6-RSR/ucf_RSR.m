clear all
clc
dbstop error;


% Sparse Coding and Dictionary Learning for Symmetric
%Positive Definite Matrices: A Kernel Approach
%ECCV 2012

addpath('/home/johanna/codes/Mehrtash/harandi_eccv_2012_matlab/');% --> Wanda

path  = '~/codes/codes-git/manifolds/trunk/ucf_sports/';
dataset_path  = '~/codes/datasets_codes/my_ucf_sports_actions/';

%home
%path_dataset = '/media/johanna/HD1T/codes/datasets_codes/my_ucf_sports_actions/';

action_seq_names = list_video_sequences(dataset_path);

dim = 14;
n_videos = 150; %From the dataset description (150). Problem with Run-Side_001_dim14
n_test =  n_videos - 2; %Problem with Run-Side_001_dim14


scale_factor = 1;
shift = 0;


%prompt = 'SR_Lambda? ';
%SR_Lambda_input = input(prompt);
%SR_Lambda_input = 1e-1;
SR_Lambda_input = 1;

%prompt = 'Beta ';
%Beta = input(prompt);
Beta = 1e1;

%vec_Beta = 1:1:10;
%all_means = zeros(length(vec_Beta),1);



%for b=1:length(vec_Beta)
    %Beta = vec_Beta(b)
    load_sub_path =strcat(path, 'dim_', int2str(dim), '/cov_matrices/one-cov-mat/scale', int2str(scale_factor), '-shift', int2str(shift) );
    acc = [];
    
    for video_ts= 1: n_videos %One Run
        X_train = zeros(dim,dim,n_test);
        labels_train = zeros(n_test,1);
        k =1;
        
        action_name = action_seq_names(video_ts,1);
        folder_n    = action_seq_names(video_ts,2);
        
        if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
            %ts = [action_name,'_',folder_n];
            %disp(ts);
            for video_tr=1: n_videos
                if (video_tr~=video_ts)
                    action_name_tr = action_seq_names(video_tr,1);
                    folder_n_tr    = action_seq_names(video_tr,2);
                    act_tr  =  str2double( action_seq_names(video_tr,3) );
                    if (~(strcmp(action_name_tr, 'Run-Side') && strcmp(folder_n_tr,'001')))
                        %tr = [action_name_tr,'_',folder_n_tr];
                        %disp(tr);
                        name_load_cov = strcat( load_sub_path, '/cov_', action_name_tr, '_', folder_n_tr, '_dim', int2str(dim), '.h5');
                        hinfo = hdf5info( char(name_load_cov) );
                        one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                        %disp(one_video);
                        X_train(:,:,k) = one_video;
                        labels_train(k) = act_tr;
                        k=k+1;
                    end
                end
            end
        end
        %Testing Set only has one video
        if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
            action_name = action_seq_names(video_ts,1);
            folder_ns    = action_seq_names(video_ts,2);
            act_ts  =  str2double( action_seq_names(video_ts,3) );
            
            labels_test = act_ts;
            %disp('Testing with: ');
            %name_load_cov = strcat( load_sub_path, '/cov_', action_name, '_', folder_n, '_dim', int2str(dim), '.h5')
            hinfo = hdf5info( char(name_load_cov) );
            one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
            X_test(:,:,1) = one_video;
            
            
            
            
            TrainSet(1).X = X_train;
            TrainSet(1).y = labels_train;
            
            TestSet(1).X = X_test;
            TestSet(1).y = labels_test;
            
            %% As per example in Code
            
            CRR = RSR(TrainSet,TestSet,Beta,SR_Lambda_input);
            %fprintf('Correct recognition accuracy with a labeled dictionary : %.1f%%.\n',100*CRR)
            
            acc = [acc CRR];
        end
    end
    
    %acc
    %size(acc)
    %all_means(b) =mean(acc)*100;
%end

