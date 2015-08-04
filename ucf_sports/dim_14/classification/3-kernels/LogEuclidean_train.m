function acc = LogEuclidean_train(path, action_seq_names, sigma, dim, n_videos)

scale_factor = 1;
shift = 0;
n_test =  n_videos - 2; %Problem with Run-Side_001_dim14

acc = [];

RIEMANNIAN_KERNEL = @(X,Y,sigma) exp( -( dist_LogEuclidean(X,Y) )^2/(2*sigma^2) );



load_sub_path =strcat(path, 'dim_', int2str(dim), '/cov_matrices/one-cov-mat/scale', int2str(scale_factor), '-shift', int2str(shift) );

for video_ts= 1: n_videos
    X_train = zeros(dim,dim,n_test);
    labels_train = zeros(n_test,1);
    k =1;
    
    action_name = action_seq_names(video_ts,1);
    folder_n    = action_seq_names(video_ts,2);
    
    
    if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
        ts = [action_name,'_',folder_n];
        disp(ts);
        for video_tr=1: n_videos
            if (video_tr~=video_ts)
                action_name_tr = action_seq_names(video_tr,1);
                folder_n_tr    = action_seq_names(video_tr,2);
                act_tr  =  str2double( action_seq_names(video_tr,3) );
                if (~(strcmp(action_name_tr, 'Run-Side') && strcmp(folder_n_tr,'001')))
                    %tr = [action_name_tr,'_',folder_n_tr];
                    %disp(tr);
                    name_load_cov = strcat( load_sub_path, '/cov_', action_name_tr, '_', folder_n_tr, '_dim', int2str(dim), '.h5');
                    hinfo = hdf5info( char(name_load_cov) );
                    one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                    %disp(one_video);
                    X_train(:,:,k) = one_video;
                    labels_train(k) = act_tr;
                    k=k+1;
                end
            end
        end
        
        
        size(K_train)
        K_train = compute_kernel_svm(X_train,X_train, RIEMANNIAN_KERNEL, sigma);
        model = svmtrain(labels_train, [[1:size(K_train,1)]' K_train], '-t 4 -q ');
        %Borrame Funciona Bien
        [predict_label, accuracy, dec_values] = svmpredict(labels_train,[[1:size(K_train,1)]' K_train], model);
        %display(accuracy');
        
        acc = [acc accuracy(1)];
        save_svm_model =strcat( './svm_models/logEucl_run_', int2str(pe_ts), '_Sigma', num2str(sigma),'.mat');
        save(save_svm_model, 'model', 'X_train');
    end
end


end

