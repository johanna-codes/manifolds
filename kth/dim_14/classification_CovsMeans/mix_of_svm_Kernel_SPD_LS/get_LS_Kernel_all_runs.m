function get_LS_Kernel_all_runs(path,dim, best_delta, best_p)

gamma = 2^best_delta/dim;
PROJECTION_RBF_KERNEL = @(X,Y,gamma) exp( -gamma*( norm(X*X'-Y*Y','fro') )^2 );






actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = 1;

acc = [];

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat(path, 'grass_points/kth-grass-point-one-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );



for pe_ts= 1: n_peo
    X=sprintf('people = %d ', pe_ts);
    disp(X);
    X_train_ls = zeros(dim,best_p,n_test);    
    labels_train = zeros(n_test,1);
    k =1;
    for pe_tr=1: n_peo
        if pe_tr~=pe_ts
            for act=1: n_actions
                
                name_load_gp = strcat( load_sub_path, '/grass_pt_', all_people(pe_tr), '_', actions(act), '_dim', int2str(dim), '_p', num2str(best_p), '.h5');

                hinfo_ls = hdf5info( char(name_load_gp) );
                one_video_ls = hdf5read(hinfo_ls.GroupHierarchy.Datasets(1));
                
                               
                X_train_ls(:,:,k) = one_video_ls;
                labels_train(k) = act;
                k=k+1;
                
            end
        end
    end
    
    K_train_ls = compute_projRGB_kernel_svm(X_train_ls,X_train_ls, PROJECTION_RBF_KERNEL, gamma);
    
    save_K_train = strcat( './Kernel_data/LSKernel_run',num2str(pe_ts), '.mat');
    save(save_K_train, 'X_train_ls', 'K_train_ls', 'labels_train');
end