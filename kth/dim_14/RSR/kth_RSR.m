clear all
clc


% Sparse Coding and Dictionary Learning for Symmetric
%Positive Definite Matrices: A Kernel Approach
%ECCV 2012

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

prompt = 'Beta ';
Beta = input(prompt);

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
    
    SPARSE_SOLVER = 'CVX'; %options are CVX and SPAMS
    
    %Generate Kernel
    galleryKernel = exp(-Beta*Stein_Divergence(TrainSet.X,TrainSet.X));
    probeKernel = exp(-Beta*Stein_Divergence(TestSet.X,TrainSet.X))';
    
    
    %Normalize dictionary
    KD = galleryKernel./repmat(sqrt(sum(galleryKernel.^2)),[size(galleryKernel,1) 1]);
    
    %
    KX = probeKernel./repmat(sqrt(sum(probeKernel.^2)),[size(probeKernel,1) 1]);
    L1 = size(probeKernel,2);
    if (strcmpi(SPARSE_SOLVER,'CVX'))
        SR_Lambda = SR_Lambda_input;
        A = KD;
        Number_Visual_Codes = size(A,2);
        scX = zeros(Number_Visual_Codes,L1);
        for tmpC1=1:L1
            if (mod(tmpC1,10) == 1)
                fprintf('Working on sample %d/%d\n',tmpC1,L1);
            end
            cvx_begin quiet
            variable tmp_Sparse_X(Number_Visual_Codes);
            minimize( -2*tmp_Sparse_X'*KX(:,tmpC1)+tmp_Sparse_X'*KD*tmp_Sparse_X+SR_Lambda*norm(tmp_Sparse_X,1));
            % if you are interested in positive sparse codes, uncomment the following two lines
            %             subject to
            %                 tmp_Sparse_X >= 0;
            cvx_end
            scX(:,tmpC1) = tmp_Sparse_X;
        end %endfor
    elseif (strcmpi(SPARSE_SOLVER,'SPAMS'))
        SR_Lambda = 1e-2;
        [KD_U,KD_D,~] = svd(KD);
        A = diag(sqrt(diag(KD_D)))*KD_U';
        D_Inv = KD_U*diag(1./sqrt(diag(KD_D)));
        KX = D_Inv'*KX;
        scX = full(mexLasso(KX,A,struct('mode',2,'lambda',SR_Lambda,'lambda2',0)));
    else
        error('Sparse solver is not defined. This version accepts CVX or SPAMS');
    end
    
    
    %% Classification
    qLabel = Classify_SRC(A,TrainSet.y,scX,KX);
    
    
    
    CRR = sum((qLabel - TestSet.y == 0))/L1;
    acc = [acc CRR];
    
    %disp('Press a Key');
    %pause
    
end
mean(acc)*100
