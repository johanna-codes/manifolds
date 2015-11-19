clear all
clc


% Dictionary Learning and Sparse Coding on
% Grassmann Manifolds: An Extrinsic Solution
% ICCV 2013
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013');% --> Wanda
addpath('/home/johanna/codes/Mehrtash/harandi_iccv_2013/iccv_ext_func');% --> Wanda

path  = '~/codes/codes-git/manifolds/trunk/kth/dim_14/';
dim = 14;



actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;
sc = 1;


n_actions = size(actions,1);
n_peo =  size(all_people,1);


SR_lambda_Vec = 0.01:0.01:1e-1*10; %sparse representation parameter
results = cell(length(SR_lambda_Vec),2);


for i=1:length(SR_lambda_Vec)
    
    SR_lambda = SR_lambda_Vec(i);   %sparse representation parameterc = 1;
    acc = [];
    
    
    load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );
    
    %Joining testing data
    n_test = (n_peo-1)*n_actions;
    
    
    for pe_ts= 1: n_peo %%One run
        X_train = zeros(dim,dim,n_test);
        labels_train = zeros(1,n_test);
        k =1;
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
        
        
        
        trn.X = X_train;
        trn.y = labels_train;
        tst.X = X_test;
        tst.y = labels_test;
        
        %% As per example in Code
        Solver_Flag = 1;  %1: SPAMS, 2: CVX
        %SPAMS toolbox is available from http://spams-devel.gforge.inria.fr/
        %CVX is available from http://cvxr.com/cvx/
        
        
        [gSC_alpha,gSC_qX,gSC_D] = gsc_func(tst.X,trn.X,SR_lambda,Solver_Flag);
        %Classification-SRC
        y_hat = Classify_SRC(gSC_D,trn.y,gSC_alpha,gSC_qX);
        CRR = sum(double(y_hat == tst.y))/length(tst.y);
        %fprintf('Correct recognition accuracy with a labeled dictionary : %.1f%%.\n',100*CRR);
        
        acc = [acc CRR];
        
        %disp('Press a Key');
        %pause
        
    end
    fprintf('Correct recognition accuracy with a labeled dictionary : %.1f%%.\n',100*mean(acc));

    results{i,1} = SR_lambda;
    results{i,2} = acc;
end





