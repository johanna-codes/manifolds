function acc = kth_train_2 (path, best_n)

gamma = 1/best_n;
LED_POLY_KERNEL = @(X,Y,gamma,best_n)( ( gamma*( trace(logm(X)'*logm(Y)) ) )^best_n );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', int2str(shift) );



for pe_ts= 1: n_peo
    X=sprintf('people = %d ', pe_ts);
    disp(X);
    
    new_features = zeros(n_test,30);    %30 = 15*2.  15 es la salida del svm 

    
    load_svm_model =strcat( './svm_models_output1/svm1_2_run',num2str(pe_ts), '.mat');
    load(load_svm_model); %loading  'model_1', 'model_2', 'X_train_covs', 'X_train_means'

    labels_train = zeros(n_test,1);
    k =1;
    for pe_tr=1: n_peo
        if pe_tr~=pe_ts
            for act=1: n_actions
                
                name_load_cov =  strcat( load_sub_path, '/Cov_', all_people(pe_tr), '_', actions(act),  '.h5');
                name_load_mean = strcat( load_sub_path, '/Means_', all_people(pe_tr), '_', actions(act),'.h5');
                
                
                hinfo_cov = hdf5info( char(name_load_cov) );
                one_video_cov = hdf5read(hinfo_cov.GroupHierarchy.Datasets(1));
                
                hinfo_mean = hdf5info( char(name_load_mean) );
                one_video_mean = hdf5read(hinfo_mean.GroupHierarchy.Datasets(1));
                                

                
                X_train_covs_1(:,:,1) = one_video_cov;
                X_train_means_1(:,:,1) = one_video_mean;

                K_train_covs =   compute_poly_kernel_svm(X_train_covs_1,X_train_covs, LED_POLY_KERNEL, gamma, best_n);
                K_train_means = compute_dot_rpoduct_kernel(X_train_means_1,X_train_means);
                
                
                [predict_label, accuracy, dec_values_1] = svmpredict([act_ts],[[1:size(K_train_covs,1)]' K_train_covs], model_1);
                [predict_label, accuracy, dec_values_2] = svmpredict([act_ts],[[1:size(K_train_means,1)]' K_train_means], model_2);
                
                comb_dec_values = [dec_values_1 dec_values_2];
                new_features(k,:) = comb_dec_values;
                labels_train(k) = act;
                k=k+1;
            end
        end
    end
    
    linear_model = svmtrain(labels_train, [[1:size(comb_dec_values,1 )]' comb_dec_values], '-t 0');
    
    save_svm_model =strcat( './svm_models_output2/linear_svm_run',num2str(pe_ts),  '.mat');
    save(save_svm_model, 'linear_model');

end