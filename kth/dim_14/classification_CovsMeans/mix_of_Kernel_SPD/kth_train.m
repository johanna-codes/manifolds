function kth_train(alpha)


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');

n_peo =  size(all_people,1);



for pe_ts= 1: n_peo
    
    
    load_K_train = strcat( './Kernel_data/Kernel_run',num2str(pe_ts), '.mat');
    load(load_K_train); % Loading 'X_train_covs', 'X_train_means', 'K_train_covs', 'K_train_means', 'labels_train
    
    
    K_train = K_train_covs + alpha*K_train_means;
    
    model = svmtrain(labels_train, [[1:size(K_train,1)]' K_train], '-t 4 -q ');
    
    save_svm_model =strcat( './svm_models_mixKernels/mixKernels_svm_run',num2str(pe_ts), '_alpha', num2str(alpha), '.mat')
    save(save_svm_model, 'model', 'X_train_covs', 'X_train_means', 'alpha');
end

