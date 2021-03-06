function acc = kth_train_LED_POLY(path,dim, n)

gamma = 1/n;
LED_POLY_KERNEL = @(X,Y,gamma,n)( ( gamma*( trace(logm(X)'*logm(Y)) ) )^n );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', int2str(shift) )



for pe_ts= 1: n_peo
    X_train = zeros(dim,dim,n_test);
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
                
                
                EmbCovMean = get_emb_LogCov(one_video_cov, one_video_mean);
                
                X_train(:,:,k) = EmbCovMean;
                labels_train(k) = act;
                k=k+1;
            end
        end
    end
    
    %disp(strcat('Doing for n= ', num2str(n)));
    K_train = compute_poly_kernel_svm(X_train,X_train, LED_POLY_KERNEL, gamma, n);
    %disp('SVM');
    %find(isfinite(K_train )==0)
    %K_train
    model = svmtrain(labels_train, [[1:size(K_train,1)]' K_train], '-t 4 -q ');
    %Borrame Funciona Bien
    %[predict_label, accuracy, dec_values] = svmpredict(labels_train,[[1:size(K_train,1)]' K_train], model);
    %display(accuracy');
    
    
    %acc = [acc accuracy(1)];
    save_svm_model =strcat( './svm_models_LED_POLY/LED-POLY_svm_run_',num2str(pe_ts), '_n', num2str(n), '.mat')
    save(save_svm_model, 'model', 'X_train');
end

