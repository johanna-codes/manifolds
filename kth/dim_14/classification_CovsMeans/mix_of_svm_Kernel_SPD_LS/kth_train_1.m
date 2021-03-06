function kth_train_1

all_people = importdata('people_list.txt');

n_peo =  size(all_people,1);



for pe_ts= 1: n_peo
    
    
    load_K_train_spd = strcat( './Kernel_data/SPDKernel_run',num2str(pe_ts), '.mat');
    load(load_K_train_spd); % Loading 'X_train_covs', , 'K_train_covs', 'labels_train
    
    load_K_ls = strcat( './Kernel_data/LSKernel_run',num2str(pe_ts), '.mat');
    load(load_K_ls) % loading 'X_train_ls', 'K_train_ls', 'labels_train');
    
   
    model_1 = svmtrain(labels_train, [[1:size(K_train_covs,1 )]' K_train_covs], '-t 4 -q ');
    model_2 = svmtrain(labels_train, [[1:size(K_train_ls,1)]' K_train_ls], '-t 4 -q ');

    save_svm_model =strcat( './svm_models_output1/svm1_2_run',num2str(pe_ts), '.mat');
    save(save_svm_model, 'model_1', 'model_2', 'X_train_covs', 'X_train_ls');
end

