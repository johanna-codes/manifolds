function acc = LogEuclidean_test(path, action_seq_names, scale_factor, shift, sigma, dim, num_videos)




RIEMANNIAN_KERNEL = @(X,Y,sigma) exp( -( dist_LogEuclidean(X,Y) )^2/(2*sigma^2) );

acc = 0;
real_labels = zeros(num_videos - 1);%Problem with Run-Side_001_dim14
est_labels  = zeros(num_videos - 1);
  

load_sub_path =strcat(path, 'dim_', int2str(dim), '/cov_matrices/one-cov-mat/scale', int2str(scale_factor), '-shift', int2str(shift) );

j=1;
  for video_ts= 1: n_videos
      action_name = action_seq_names(video_ts,1);
      folder_n    = action_seq_names(video_ts,2);
      act_ts  =  str2double( action_seq_names(video_tr,3) );
      
      if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
          load_svm_model = strcat( './svm_models/logEucl_run_', int2str(video_ts), '_Sigma', num2str(sigma),'.mat');
          load(load_svm_model); %loading model and X_train
      
          real_labels(j) = act_ts;
          
          name_load_cov = strcat( load_sub_path, '/cov_', action_name, '_', folder_n, '_dim', int2str(dim), '.h5');
          hinfo = hdf5info( char(name_load_cov) );
          one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
          X_test(:,:,1) = one_video;
          
          K_test = compute_kernel_svm(X_test,X_train, RIEMANNIAN_KERNEL,sigma);
          [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
          est_labels(j) = predict_label;
          j=j+1;
          
          if predict_label == act_ts
              acc = acc+1;
          end

      end
      
     save_labels = strcat('./svm_results/LogEucl_scale', int2str(scale_factor), '-shift', int2str(shift),'-sigma',num2str(sigma),'.mat' );     
     save(save_labels, 'est_labels', 'real_labels', 'sigma');
  
  end
  %[acc n_peo n_actions]
  acc = acc*100/(n_peo*n_actions);
   
