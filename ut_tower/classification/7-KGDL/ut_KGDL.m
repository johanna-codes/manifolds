clear all
clc
dbstop error;


% Dictionary Learning and Sparse Coding on
% Grassmann Manifolds: An Extrinsic Solution
% ICCV 2013

addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013');% --> Wanda
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013/iccv_ext_func');% --> Wanda

path  = '~/codes/codes-git/manifolds/trunk/ut_tower/'; % --> Wanda
%home
%path_dataset = '/media/johanna/HD1T/codes/datasets_codes/my_ucf_sports_actions/';


action_seq_names = list_video_sequences;

dim =14;
num_videos = 108; %From the dataset description (108).
n_test =  num_videos - 1; 


scale_factor = 1;
shift = 0;

%prompt = 'best p?';
%best_p= input(prompt);
best_p = 2;


Solver_Flag = 1;  %1: SPAMS, 2: CVX

%SR_lambda = 1e-3;    %sparse representation parameter
nAtoms = 128;        %size of the dictionary
dict_options.L = 20; %number of non-zero elements in OMP for dictionary learning


SR_lambda_vec = 0.1:0.1:1;
nAtoms_vec = [128];
all_means=zeros(length(SR_lambda_vec ),1);


load_sub_path =strcat(path, 'grass_points/one-grass-point/scale', int2str(scale_factor), '-shift', int2str(shift) );


for sr=1:length(nAtoms_vec )
    
    SR_lambda =   SR_lambda_vec(sr);
    
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
                name_load_gp = strcat( load_sub_path, '/grass_pt_',  action_name_tr, '_seq', seqID_tr, '_p', int2str(best_p),  '.h5');
                hinfo_tr = hdf5info( char(name_load_gp) );
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
        
        name_load_gp_ts = strcat( load_sub_path, '/grass_pt_',  action_name_ts, '_seq', seqID_ts, '_p', int2str(best_p),  '.h5');
        hinfo_ts = hdf5info( char(name_load_gp_ts) );
        one_video_ts = hdf5read(hinfo_ts.GroupHierarchy.Datasets(1));
        
        labels_test = act_ts;
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
    all_means (sr) = mean(acc)*100;
    
end


