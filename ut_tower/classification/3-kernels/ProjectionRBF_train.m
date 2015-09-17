function acc = ProjectionRBF_train(path, action_seq_names, delta, dim, p, n_videos)

scale_factor = 1;
shift = 0;
n_test =  n_videos - 1; 
acc = [];

gamma = 2^delta/dim;
PROJECTION_RBF_KERNEL = @(X,Y,gamma) exp( -gamma*( norm(X*X'-Y*Y','fro') )^2 );


load_sub_path =strcat(path, '/grass_points/one-grass-point/scale', int2str(scale_factor), '-shift', int2str(shift) );

%parpool(5);
for video_ts= 1: n_videos
    X_train = zeros(dim,p,n_test);
    labels_train = zeros(n_test,1);
    k =1;

        for video_tr=1: n_videos
            if (video_tr~=video_ts)
                action_name_tr = action_seq_names(video_tr,1);
                act_tr  =  str2double( action_seq_names(video_tr,2) );
                seqID_tr    = action_seq_names(video_tr,3);
                    name_load_gp = strcat( load_sub_path, '/grass_pt_',  action_name_tr, '_seq', seqID_tr, '.h5');
                    hinfo = hdf5info( char(name_load_gp) );
                    one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
                    %disp(one_video);
                    X_train(:,:,k) = one_video;
                    labels_train(k) = act_tr;
                    k=k+1;
            end
        end
        
        K_train = compute_kernel_svm(X_train,X_train, PROJECTION_RBF_KERNEL, gamma);
        model = svmtrain(labels_train, [[1:size(K_train,1)]' K_train], '-t 4 -q ');
        %Borrame Funciona Bien
        [predict_label, accuracy, dec_values] = svmpredict(labels_train,[[1:size(K_train,1)]' K_train], model);
        %display(accuracy');
        
        acc = [acc accuracy(1)];
        save_svm_model =strcat( './svm_models_ProjectionRBF/projRBF_run_', int2str(video_ts), '_p', int2str(p), '_delta', num2str(delta),'.mat');
        parfor_save(save_svm_model, model, X_train);
        %save(save_svm_model, 'model', 'X_train');
end


end

