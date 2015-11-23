clear all
clc
dbstop error;


% Dictionary Learning and Sparse Coding on
% Grassmann Manifolds: An Extrinsic Solution
% ICCV 2013

addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013');% --> Wanda
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013/iccv_ext_func');% --> Wanda

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

prompt = 'best p?';
best_p= input(prompt);
%best_p = 2;


Solver_Flag = 1;  %1: SPAMS, 2: CVX

SR_lambda = 1e-1;    %sparse representation parameter
nAtoms = 128;        %size of the dictionary
dict_options.L = 20; %number of non-zero elements in OMP for dictionary learning



acc = [];



load_sub_path =strcat(path, 'dim_', int2str(dim), '/grass_points/one-grass-point/scale', num2str(scale_factor), '-shift', int2str(shift) );


for video_ts= 1: n_videos %One Run
    X_train = zeros(dim,best_p,n_test);
    labels_train = zeros(1,n_test);
    k =1;
    
    action_name = action_seq_names(video_ts,1);
    folder_n    = action_seq_names(video_ts,2);
    
    if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
        %ts = [action_name,'_',folder_n];
        %disp(ts);
        %Joining all The Trainin Set in Matrix X_train
        for video_tr=1: n_videos
            if (video_tr~=video_ts)
                action_name_tr = action_seq_names(video_tr,1);
                folder_n_tr    = action_seq_names(video_tr,2);
                act_tr  =  str2double( action_seq_names(video_tr,3) );
                if (~(strcmp(action_name_tr, 'Run-Side') && strcmp(folder_n_tr,'001')))
                    %tr = [action_name_tr,'_',folder_n_tr];
                    %disp(tr);
                    name_load_gp = strcat( load_sub_path, '/grass_pt_', action_name_tr, '_', folder_n_tr, '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
                    hinfo = hdf5info( char(name_load_gp) );
                    one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                    %disp(one_video);
                    X_train(:,:,k) = one_video;
                    labels_train(k) = act_tr;
                    k=k+1;
                end
            end
        end
        
        %Testing Set only has one video
        action_name_ts = action_seq_names(video_ts,1);
        folder_n_ts    = action_seq_names(video_ts,2);
        act_ts  =  str2double( action_seq_names(video_ts,3) );
        X_test = zeros(dim,best_p,1);
         
        labels_test = act_ts;
        %disp('Testing with: ');
        name_load_gp_ts = strcat( load_sub_path, '/grass_pt_', action_name_ts, '_', folder_n_ts, '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
        hinfo_ts = hdf5info( char(name_load_gp_ts) );
        one_video_ts = hdf5read(hinfo_ts.GroupHierarchy.Datasets(1));
        X_test(:,:,1) = one_video_ts;
        
        
        trn.X = X_train;
        trn.y = labels_train;
        tst.X = X_test;
        tst.y = labels_test;
        
        %% As per example in Code
        
        
        CRR = KGDL(trn, tst, Solver_Flag, SR_lambda,nAtoms,dict_options);
        fprintf('Correct recognition accuracy with a labeled dictionary : %.1f%%.\n',100*CRR);
        acc = [acc CRR];
    end
end

fprintf('Cross Validation Accuracy: %.1f%%.\n',mean(acc)*100);

[SR_lambda nAtoms dict_options.L best_p]



