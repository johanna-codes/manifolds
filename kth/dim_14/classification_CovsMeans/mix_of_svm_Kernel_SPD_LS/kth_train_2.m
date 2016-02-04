function acc = kth_train_2 (path, best_n, c, best_delta, best_p)

gamma_spd = 1/best_n;
LED_POLY_KERNEL = @(X,Y,gamma_spd,best_n)( ( gamma_spd*( trace(logm(X)'*logm(Y)) ) )^best_n );

gamma_ls = 2^best_delta/dim;
PROJECTION_RBF_KERNEL = @(X,Y,gamma_ls) exp( -gamma_ls*( norm(X*X'-Y*Y','fro') )^2 );



actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;

load_sub_path_1 =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', int2str(shift) );
load_sub_path_2 =strcat(path, 'grass_points/kth-grass-point-one-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );




for pe_ts= 1: n_peo
    X=sprintf('people = %d ', pe_ts);
    disp(X);
    
    new_features = zeros(n_test,30);    %30 = 15*2.  15 es la salida del svm 
    
    load_svm_model =strcat( './svm_models_output1/svm1_2_run',num2str(pe_ts), '.mat');
    load(load_svm_model); %loading  'model_1', 'model_2', 'X_train_covs', 'X_train_ls'

    labels_train = zeros(n_test,1);
    k =1;
    for pe_tr=1: n_peo
        if pe_tr~=pe_ts
            for act=1: n_actions
                
                name_load_cov =  strcat( load_sub_path_1, '/Cov_', all_people(pe_tr), '_', actions(act),  '.h5');
                hinfo_cov = hdf5info( char(name_load_cov) );
                one_video_cov = hdf5read(hinfo_cov.GroupHierarchy.Datasets(1));
                X_train_covs_1(:,:,1) = one_video_cov;                
                K_train_covs =   compute_poly_kernel_svm(X_train_covs_1,X_train_covs, LED_POLY_KERNEL, gamma_spd, best_n);
                [predict_label, accuracy, dec_values_1] = svmpredict([act],[[1:size(K_train_covs,1)]' K_train_covs], model_1);
                
                
                name_load_gp = strcat( load_sub_path_2, '/grass_pt_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '_p', num2str(best_p), '.h5');
                hinfo_ls = hdf5info( char(name_load_gp) );
                one_video_ls = hdf5read(hinfo_ls.GroupHierarchy.Datasets(1));
                X_train_ls_1(:,:,1) = one_video_ls;
                K_train_ls = compute_projRGB_kernel_svm(X_train_ls_1,X_train_ls, PROJECTION_RBF_KERNEL, gamma_ls);

                
                [predict_label, accuracy, dec_values_2] = svmpredict([act],[[1:size(K_train_ls,1)]' K_train_ls], model_2);
                
                
                comb_dec_values = [dec_values_1 dec_values_2];
                new_features(k,:) = comb_dec_values;
                labels_train(k) = act;
                k=k+1;
            end
        end
    end
    
    %param = ['-q -v 3 -c ', num2str(2^log2c), ' -g ', num2str(2^log2g)];
    param = ['-t 0 -q -c ',  num2str(c)];
    %pause
    
    linear_model = svmtrain(labels_train, [new_features], param);
    %pause
    save_svm_model =strcat( './svm_models_output2/linear_svm_run',num2str(pe_ts),  '.mat');
    save(save_svm_model, 'linear_model');

end