function acc = kth_test_LED_POLY(path,scale_factor, shift, dim, n)

gamma = 1/n;
LED_POLY_KERNEL = @(X,Y,gamma,n)( ( gamma*( trace(logm(X)'*logm(Y)) ) )^n );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = 0;
real_labels = zeros(n_peo*n_actions);
est_labels  = zeros(n_peo*n_actions);

load_sub_path =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', num2str(shift) )
%load_sub_path =strcat(path, 'covs_means_matrices/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', num2str(shift) )

j=1;
for pe_ts= 1: n_peo
    
    load_svm_model =strcat( './svm_models_LED_POLY/LED-POLY_svm_run_',num2str(pe_ts), '_n', num2str(n),  '.mat');
    load(load_svm_model); %loading model and X_train
    
    for act_ts = 1:n_actions
        
        real_labels(j) = act_ts;
        
        name_load_cov =  strcat( load_sub_path, '/Cov_', all_people(pe_ts), '_', actions(act_ts),  '.h5');
        name_load_mean = strcat( load_sub_path, '/Means_', all_people(pe_ts), '_', actions(act_ts),'.h5');
        
        
        hinfo_cov = hdf5info( char(name_load_cov) );
        one_video_cov = hdf5read(hinfo_cov.GroupHierarchy.Datasets(1));
        
        hinfo_mean = hdf5info( char(name_load_mean) );
        one_video_mean = hdf5read(hinfo_mean.GroupHierarchy.Datasets(1));
        
        EmbCovMean = get_emb_LogCov(one_video_cov, one_video_mean);
        
        
        
        
        X_test(:,:,1) = EmbCovMean;
        
        %size(X_test)
        %size(X_train)
        K_test = compute_poly_kernel_svm(X_test,X_train, LED_POLY_KERNEL,gamma,n);
        %size (K_test)
        [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
        est_labels(j) = predict_label;
        j=j+1;
        
        if predict_label == act_ts
            acc = acc+1;
        end
        
    end
    
    save_labels = strcat('./svm_results_LED_POLY/LED-POLY_scale', num2str(scale_factor), '-shift', num2str(shift),'-n',num2str(n),'.mat' );
    save(save_labels, 'est_labels', 'real_labels', 'n');
    
end
%[acc n_peo n_actions]
acc = acc*100/(n_peo*n_actions);

