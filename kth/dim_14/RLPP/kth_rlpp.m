clear all
clc


% Kernel Analysis over Riemannian Manifolds
%for Visual Recognition of Actions, Pedestrians and Texture
%WACV 2012
addpath('/home/johanna/codes/Mehrtash/harandi_wacv_2012_matlab/WACV');% --> Wanda



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
    
    TrainSet(1).X = X_train;
    TrainSet(1).y = labels_train;
    
    TestSet(1).X = X_test;
    TestSet(1).y = labels_test;
    
    %% As per example in Code
    R_Kernel_Sigma = 5e+0; %Gaussian parameter in Riemannian kernel, (equation (5) in paper).
    
    %% RLPP
    %Computing the kernels
    [Train_Kernel]=Compute_Riemannian_Kernel(TrainSet.X,TrainSet.X,R_Kernel_Sigma);
    % Generating adjacency graph
    k_W = 3; %number of nearest neighbors
    W = Generate_Graph(struct('X',Train_Kernel,'y',TrainSet.y),k_W);
    
    %Learning the Riemannian locality preserving projection
    RLPP_Sigma = 1e1;
    Projection_Model=RLPP(Train_Kernel,W,RLPP_Sigma);
    
    %% Projecting train and test data by the RLPP model
    %Train
    FTrain_Proj=Projection_Model.Alpha'*Train_Kernel;
    
    %Test
    [Test_Kernel]=Compute_Riemannian_Kernel(TrainSet.X,TestSet.X,R_Kernel_Sigma);
    FTest_Proj=Projection_Model.Alpha'*Test_Kernel;
    
    %% Nearest Neighbour Clasfication
    CRR = myNN(struct('X',FTest_Proj,'y',TestSet.y),struct('X',FTrain_Proj,'y',TrainSet.y))
    
    
    disp('Press a Key');
    pause
    
end

