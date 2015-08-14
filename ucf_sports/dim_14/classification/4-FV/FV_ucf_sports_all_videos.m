function FV_ucf_sports_all_videos(path, Ncent, DIM, scale_factor, shift, n_videos, action_seq_names )
%%Calcular FV for all videos

%n_test =  n_videos - 1; %Problem with Run-Side_001_dim14

fprintf('Ng %d \n',Ncent);


Ng = int2str(Ncent);
dim = int2str(DIM);


w  =    load(strcat('./universal_GMM/weights_Ng', Ng, '_dim', dim, '.dat'));
mu =    load(strcat('./universal_GMM/means_Ng'  , Ng, '_dim', dim, '.dat'));
sigma = load(strcat('./universal_GMM/covs_Ng'   , Ng, '_dim', dim, '.dat'));


folder_feat =strcat(path, 'dim_', dim, '/features/features_dim', dim, '/scale', int2str(scale_factor), '-shift', int2str(shift) );


for video_ts= 1: n_videos
   action_name = action_seq_names(video_ts,1);
   folder_n    = action_seq_names(video_ts,2);
    
    
    if (~(strcmp(action_name,'Run-Side') && strcmp(folder_n,'001')))
        name_feat = strcat( folder_feat, '/', action_name, '_', folder_n, '_dim', dim, '.h5');
        S = char(name_feat)
        data_onevideo = hdf5info(S);
        one_video = hdf5read(data_onevideo.GroupHierarchy.Datasets(1));
        one_video= {one_video};
        v = compute_fisher_joha (single(w), single(mu), single(sigma), one_video);
        
        d_fisher = size (v, 1);              % dimension of the Fisher vectors
        
        % power "normalisation"
        v = sign(v) .* sqrt(abs(v));
        
        %L2 normalization (may introduce NaN vectors)
        vn = yael_fvecs_normalize (v);
        
        % replace NaN vectors with a large value that is far from everything else
        % For normalized vectors in high dimension, vector (0, ..., 0) is *close* to
        % many vectors.
        %vn(find(isnan(vn))) = 123;
        
        if ( length( find( isnan(vn) ) )> 0 )
            disp('Que hago??????');
            
        end
        %to save
        
        save_name = strcat('./FV_training/scale',int2str(scale_factor), '-shift',  int2str(shift),  '/FV_', action_name, '_', folder_n, '_Ng', Ng, '.txt');
        sSave = char(save_name);
        %display(sSave);
        fid1=fopen(sSave,'wt');
        fprintf(fid1,'%8.8f\n',vn);
        fclose(fid1);

    end
end





