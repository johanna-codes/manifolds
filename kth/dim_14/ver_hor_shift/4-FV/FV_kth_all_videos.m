function FV_kth_all_videos(Ncent, DIM, scale_factor, shift,flag_shift)
%%Calcular FV for all videos


path  = '/home/johanna/codes/codes-git/manifolds/trunk/kth/';
fprintf('Ng %d \n',Ncent);


Ng = int2str(Ncent);
dim = int2str(DIM);


w  =    load(strcat('./universal_GMM/weights_Ng', Ng, '_dim', dim,'_sc1.dat'));
mu =    load(strcat('./universal_GMM/means_Ng'  , Ng, '_dim', dim,  '_sc1.dat'));
sigma = load(strcat('./universal_GMM/covs_Ng'   , Ng, '_dim', dim,'_sc1.dat'));


sc = int2str(1); %Using only scenario 1

people= importdata(strcat('people_list.txt'));
actionNames = importdata('actionNames.txt');
n_people  = length(people);
n_actions = length(actionNames);

for i=1:n_people
    for j=1:n_actions
        
        if (flag_shift==true)
            folder_feat = strcat( path, 'dim_', dim, '/features/kth-features_dim', dim, '_openMP/sc', sc, '/scale',num2str(scale_factor), '-horshift',  int2str(shift) );
        end
        
        
    if (flag_shift==false)
        folder_feat = strcat( path, 'dim_', dim, '/features/kth-features_dim', dim, '_openMP/sc', sc, '/scale',num2str(scale_factor), '-vershift',  int2str(shift) );
    end
        
        
        name_feat = strcat(folder_feat, '/',  people (i),  '_', actionNames(j), '_dim', dim, '.h5');
          
        %show_you = strcat(people (i),  '_', actionNames(j));
        %disp(show_you);
        
        S = char(name_feat);
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
        
        
        if (flag_shift==true)
        save_name = strcat('./FV_training/scale',num2str(scale_factor), '-horshift',  int2str(shift),  '/FV_', people(i),'_',actionNames(j),'_sc', sc, '_Ng', Ng, '.txt');
        end
        
         if (flag_shift==false)
        save_name = strcat('./FV_training/scale',num2str(scale_factor), '-vershift',  int2str(shift),  '/FV_', people(i),'_',actionNames(j),'_sc', sc, '_Ng', Ng, '.txt');
        end
        
        
        sSave = char(save_name);
        %display(sSave);
        fid1=fopen(sSave,'wt');
        fprintf(fid1,'%8.8f\n',vn);
        fclose(fid1);
    end
    
end


