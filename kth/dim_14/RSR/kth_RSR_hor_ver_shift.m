clear all
clc


% Sparse Coding and Dictionary Learning for Symmetric
%Positive Definite Matrices: A Kernel Approach
%ECCV 2012
dbstop error;

addpath('/home/johanna/codes/Mehrtash/harandi_eccv_2012_matlab/');% --> Wanda



path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;



actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;
acc = [];


load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

%Joining testing data
n_test = (n_peo-1)*n_actions;


%prompt = 'SR_Lambda? ';
%SR_Lambda_input = input(prompt);
SR_Lambda_input = 1e-1;

%prompt = 'Beta ';
%Beta = input(prompt);
%vec_Beta = 1:1:10;
best_Beta = 1;
%all_means = zeros(length(vec_Beta),1);

%for b=1:length(vec_Beta) %% Change for Hor_ver Shift
    Beta = best_Beta;
    for pe_ts= 1: n_peo %%One run
        X_train = zeros(dim,dim,n_test);
        labels_train = zeros(1,n_test);
        k =1;
        
        %Joining Testing Data
        for pe_tr=1: n_peo
            if pe_tr~=pe_ts
                for act=1: n_actions
                    name_load_cov = strcat( load_sub_path, '/cov_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '.h5');
                    hinfo = hdf5info( char(name_load_cov) );
                    one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                    X_train(:,:,k) = one_video;
                    labels_train(k) = act;
                    k=k+1;
                end
            end
        end
        
        %Joining testing data
        j=1;
        labels_test = zeros(1,n_actions); %I test with one person and all his/hers actions
        X_test = zeros(dim,dim,n_actions);
        for act_ts = 1:n_actions
            
            name_load_cov = strcat( load_sub_path, '/cov_', all_people(pe_ts), '_', actions(act_ts), '_dim', int2str(dim), '.h5');
            %char(name_load_cov)
            hinfo = hdf5info( char(name_load_cov) );
            one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
            X_test(:,:,j) = one_video;
            labels_test(j) = act_ts;
            j = j+1;
        end
        
        
        %Creating Sets
        TrainSet(1).X = X_train;
        TrainSet(1).y = labels_train;
        
        TestSet(1).X = X_test;
        TestSet(1).y = labels_test;
        
        %% As per example in Code. RSR
        CRR = RSR(TrainSet,TestSet,Beta,SR_Lambda_input);

        acc = [acc CRR];
        
        %disp('Press a Key');
        %pause
        
    end
    %all_means(b) = mean(acc)*100;
%end