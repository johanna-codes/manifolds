function get_Kernel_all_runs(path,dim, best_n)

gamma = 1/best_n;
LED_POLY_KERNEL = @(X,Y,gamma,best_n)( ( gamma*( trace(logm(X)'*logm(Y)) ) )^best_n );


actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat(path, 'covs_means_matrices_vectors/CovMeans/sc', int2str(sc), '/scale', num2str(scale_factor), '-shift', int2str(shift) );



for pe_ts= 1: n_peo
    X_train_covs = zeros(dim,dim,n_test);    
    X_train_means = zeros(dim,1,n_test);

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
                                
                X_train_covs(:,:,k) = one_video_cov;
                X_train_means(:,:,k) = one_video_mean;
                
                
                labels_train(k) = act;
                k=k+1;
            end
        end
    end
    
    %disp(strcat('Doing for n= ', num2str(n)));
    K_train_covs = compute_poly_kernel_svm(X_train_covs,X_train_covs, LED_POLY_KERNEL, gamma, best_n);
    K_train_means = compute_dot_rpoduct_kernel(X_train_means,X_train_means);
    
    
    save_K_train = strcat( './Kernel_data/Kernel_run',num2str(pe_ts), '.mat');
    save(save_K_train, 'X_train_covs', 'X_train_means', 'K_train_covs', 'K_train_means', 'labels_train');
end