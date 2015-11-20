clear all
clc


% Dictionary Learning and Sparse Coding on
% Grassmann Manifolds: An Extrinsic Solution
% ICCV 2013
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013');% --> Wanda
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013/iccv_ext_func');% --> Wanda

path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;
best_p = 7; %For KTH. Taken from the experiments with the KTH.


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;
sc = 1;


n_actions = size(actions,1);
n_peo =  size(all_people,1);


SR_lambda_Vec = 1:1:10; %sparse representation parameter
results = cell(length(SR_lambda_Vec),2);


%for i=1:length(SR_lambda_Vec)
    
    %SR_lambda = SR_lambda_Vec(i);   %sparse representation parameterc = 1;
    acc = [];
    
    
    load_sub_path =strcat(path, 'grass_points/kth-grass-point-one-dim', int2str(dim), '/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', int2str(shift) );

    %Joining testing data
    n_test = (n_peo-1)*n_actions;
    
    
    for pe_ts= 1: n_peo %%One run
        X_train = zeros(dim,best_p,n_test);
        labels_train = zeros(1,n_test);
        k =1;
        for pe_tr=1: n_peo
            if pe_tr~=pe_ts
                for act=1: n_actions
                    name_load_gp = strcat( load_sub_path, '/grass_pt_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
                    hinfo = hdf5info( char(name_load_gp) );
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
        X_test = zeros(dim,best_p,n_actions);
        for act_ts = 1:n_actions
            name_load_gp = strcat( load_sub_path, '/grass_pt_', all_people(pe_tr), '_', actions(act_ts), '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
            hinfo = hdf5info( char(name_load_gp) );
            one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
            X_test(:,:,j) = one_video;    
            labels_test(j) = act_ts;
            j = j+1;
        end
        
        
        
        trn.X = X_train;
        trn.y = labels_train;
        tst.X = X_test;
        tst.y = labels_test;
        
        %% As per example in Code
        Solver_Flag = 1;  %1: SPAMS, 2: CVX

        SR_lambda = 1e-1;    %sparse representation parameter
        nAtoms = 64;        %size of the dictionary
        dict_options.L = 20; %number of non-zero elements in OMP for dictionary learning

        
        fprintf('Learning the Grassmannian dictionary\n');
        D = grassmann_dictionary_learning(trn.X,nAtoms,dict_options);
        [gSC_alpha_trn,~,~] = gsc_func(trn.X,D,SR_lambda,Solver_Flag);
        [gSC_alpha_tst,~,~] = gsc_func(tst.X,D,SR_lambda,Solver_Flag);
        
        % %Classification
        %Ridge regression
        nClasses = max(trn.y);
        nPoints = length(trn.y);
        L = zeros(nClasses,nPoints);
        L(sub2ind([nClasses,nPoints], trn.y, 1:nPoints)) = 1;
        zeta = 1e-1;    %regularization parameter for ridge regression
        big_alpha = gSC_alpha_trn*gSC_alpha_trn' + zeta*eye(nAtoms);
        big_v = L*gSC_alpha_trn';
        W = big_v/big_alpha;
        [~,y_hat] = max(W*gSC_alpha_tst);
        CRR = sum(y_hat == tst.y)/length(y_hat);



        acc = [acc CRR];
        
        %disp('Press a Key');
        %pause
        
    end
        fprintf('Correct recognition accuracy with a dictionary of size %d : %.1f%%.\n',nAtoms,100*mean(acc));

    %results{i,1} = SR_lambda;
    %results{i,2} = acc;
%end





