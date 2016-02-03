function kth_train(alpha, beta, nor_type)


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');

n_peo =  size(all_people,1);



for pe_ts= 1: n_peo
    
    
    load_K_train = strcat( './Kernel_data/Kernel_run',num2str(pe_ts), '.mat');
    load(load_K_train); % Loading 'X_train_covs', 'X_train_means', 'K_train_covs', 'K_train_means', 'labels_train
    
    %Normalising Kernels
    %K_train_covs  = normalise_kernel(K_train_covs, nor_type);
    %K_train_means = normalise_kernel(K_train_means, nor_type);
    
    %K_train_covs
    %K_train_means
    %K_train = beta*K_train_covs + alpha*K_train_means;
    
    model_1 = svmtrain(labels_train, [[1:size(K_train_covs,1 )]' K_train_covs], '-t 4 -q ');
    model_2 = svmtrain(labels_train, [[1:size(K_train_means,1)]' K_train_means], '-t 4 -q ');

    save_svm_model =strcat( './svm_models_mixKernels/mixKernels_svm_run',num2str(pe_ts), '_alpha', num2str(alpha), '.mat');
    save(save_svm_model, 'model_1', 'model_2', 'X_train_covs', 'X_train_means', 'alpha');
end

