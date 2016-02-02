function acc = kth_test(path,scale_factor, shift, dim, best_n, alpha, beta, nor_type)

gamma = 1/best_n;
LED_POLY_KERNEL = @(X,Y,gamma,best_n)( ( gamma*( trace(logm(X)'*logm(Y)) ) )^best_n );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = 0;
real_labels = zeros(n_peo*n_actions,1);
est_labels  = zeros(n_peo*n_actions,1);

load_sub_path =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', num2str(shift) );
%load_sub_path =strcat(path, 'covs_means_matrices/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', num2str(shift) )

j=1;
for pe_ts= 1: n_peo
    
    load_svm_model =strcat( './svm_models_mixKernels/mixKernels_svm_run',num2str(pe_ts), '_alpha', num2str(alpha), '.mat');
    load(load_svm_model); %loading model, X_train_covs and X_train_means'
    
    for act_ts = 1:n_actions
        
        real_labels(j,1) = act_ts;
        
        name_load_cov =  strcat( load_sub_path, '/Cov_', all_people(pe_ts), '_', actions(act_ts),  '.h5');
        name_load_mean = strcat( load_sub_path, '/Means_', all_people(pe_ts), '_', actions(act_ts),'.h5');
        
        
        hinfo_cov = hdf5info( char(name_load_cov) );
        one_video_cov = hdf5read(hinfo_cov.GroupHierarchy.Datasets(1));
        
        hinfo_mean = hdf5info( char(name_load_mean) );
        one_video_mean = hdf5read(hinfo_mean.GroupHierarchy.Datasets(1));
        
       
        X_test_covs(:,:,1) = one_video_cov;
        X_test_means(:,:,1) = one_video_mean;
        
        K_test_covs =   compute_poly_kernel_svm(X_test_covs,X_train_covs, LED_POLY_KERNEL, gamma, best_n);
        K_test_means = compute_dot_rpoduct_kernel(X_test_means,X_train_means);
        
        
        %Normalising Kernels
        K_test_covs  = normalise_kernel(K_test_covs, nor_type);
        K_test_means = normalise_kernel(K_test_means, nor_type);
        
        K_test= beta*K_test_covs + alpha*K_test_means;

        
        [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
        est_labels(j,1) = predict_label;
        j=j+1;
        
        [predict_label  act_ts]
        
        if predict_label == act_ts
            acc = acc+1;
        end
        
    end
    
    %save_labels = strcat('./svm_results/mixKernels_scale', num2str(scale_factor), '-shift', num2str(shift),'-alpha',num2str(alpha),'.mat' );
    %save(save_labels, 'est_labels', 'real_labels', 'alpha');
    
end
%[acc n_peo n_actions]
acc = acc*100/(n_peo*n_actions);


