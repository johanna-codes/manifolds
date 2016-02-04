function acc = kth_test(path,scale_factor, shift, best_n, best_delta, best_p)

gamma_spd = 1/best_n;
LED_POLY_KERNEL = @(X,Y,gamma_spd,best_n)( ( gamma_spd*( trace(logm(X)'*logm(Y)) ) )^best_n );

gamma_ls = 2^best_delta/dim;
PROJECTION_RBF_KERNEL = @(X,Y,gamma_ls) exp( -gamma_ls*( norm(X*X'-Y*Y','fro') )^2 );

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = 0;
real_labels = zeros(n_peo*n_actions,1);
est_labels  = zeros(n_peo*n_actions,1);

load_sub_path_1 =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', num2str(shift) );
load_sub_path_2 =strcat(path, 'grass_points/kth-grass-point-one-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );


j=1;
for pe_ts= 1: n_peo
    
    load_svm_models =strcat( './svm_models_output1/svm1_2_run',num2str(pe_ts),  '.mat');
    load(load_svm_models); %loading model_1, model_2, X_train_covs', 'X_train_ls'
    
    load_linear_svm_model = strcat( './svm_models_output2/linear_svm_run',num2str(pe_ts),  '.mat');
    load(load_linear_svm_model); %linear_model
    
    
    
    for act_ts = 1:n_actions
        
        real_labels(j,1) = act_ts;        
        name_load_cov =  strcat( load_sub_path_1, '/Cov_', all_people(pe_ts), '_', actions(act_ts),  '.h5');
        hinfo_cov = hdf5info( char(name_load_cov) );
        one_video_cov = hdf5read(hinfo_cov.GroupHierarchy.Datasets(1));        
        X_test_covs(:,:,1) = one_video_cov;
        K_test_covs =   compute_poly_kernel_svm(X_test_covs,X_train_covs, LED_POLY_KERNEL, gamma_spd, best_n);
        %Prediction for Model 1. Kernel with SPD matrices
        [predict_label, accuracy, dec_values_1] = svmpredict([act_ts],[[1:size(K_test_covs,1)]' K_test_covs], model_1);
        
        
        name_load_gp = strcat( load_sub_path_2, '/grass_pt_', all_people(pe_ts), '_', actions(act_ts), '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
        hinfo_ls = hdf5info( char(name_load_gp) );
        one_video_ls = hdf5read(hinfo_ls.GroupHierarchy.Datasets(1));
        X_test_ls_1(:,:,1) = one_video_ls;
        K_test_ls = compute_projRGB_kernel_svm(X_test_ls_1,X_train_ls, PROJECTION_RBF_KERNEL, gamma_ls);
        %Prediction for Model 1. LS Kernel
        [predict_label, accuracy, dec_values_2] = svmpredict([act_ts],[[1:size(K_test_ls,1)]' K_test_ls], model_2);

        
        comb_dec_values = [dec_values_1 dec_values_2];
        new_features = comb_dec_values;
        
        [predict_label, accuracy, new_dec_values] = svmpredict([act_ts],[new_features], linear_model);

        
        j=j+1;
        
        if predict_label == act_ts
                acc = acc+1;
        end
        
        
        
%         if my_label ~= predict_label
%             disp(' It is different. What have you done wrong')
%             pause
%         end
        
%         if my_label == predict_label
%             [my_label predict_label]
%         end
%         
    end

    
end
%[acc n_peo n_actions]
acc = acc*100/(n_peo*n_actions);


