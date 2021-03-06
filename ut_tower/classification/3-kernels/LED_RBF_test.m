function acc = LED_RBF_test(path, action_seq_names, scale_factor, shift, dim, delta, num_videos)

gamma = 1/dim*2^delta;

RIEMANNIAN_KERNEL = @(X,Y,gamma) exp( -gamma*( norm(logm(X) - logm(Y),'fro') )^2 );

acc = 0;
real_labels = zeros(num_videos);
est_labels  = zeros(num_videos);
  

load_sub_path =strcat(path, '/cov_matrices/one-cov-mat/scale', int2str(scale_factor), '-shift', int2str(shift) );

j=1;
  for video_ts= 1: num_videos
      
      action_name = action_seq_names(video_ts,1);
      act_ts  =  str2double( action_seq_names(video_ts,2) );
      seqID_ts    = action_seq_names(video_ts,3);
      
      
          load_svm_model = strcat( './svm_models_LED_RBF/LED_RBF_run_', int2str(video_ts), '_delta', num2str(delta),'.mat');
          load(load_svm_model); %loading model and X_train
      
          real_labels(j) = act_ts;
          
          name_load_cov = strcat( load_sub_path, '/cov_', action_name, '_seq', seqID_ts, '.h5');          hinfo = hdf5info( char(name_load_cov) );
          one_video = hdf5read(hinfo.GroupHierarchy.Datasets(1));
          X_test(:,:,1) = one_video;
          
          K_test = compute_kernel_svm(X_test,X_train, RIEMANNIAN_KERNEL,gamma);
          [predict_label, accuracy, dec_values] = svmpredict([act_ts],[[1:size(K_test,1)]' K_test], model);
          est_labels(j) = predict_label;
          j=j+1;
          
          if predict_label == act_ts
              acc = acc+1;
          end

      
     save_labels = strcat('./svm_results_LED_RBF/LED-RBF_scale', int2str(scale_factor), '-shift', int2str(shift),'-delta',num2str(delta),'.mat' );     
     save(save_labels, 'est_labels', 'real_labels', 'delta');
  
  end
  %[acc n_peo n_actions]
  acc = acc*100/(num_videos);
   
