%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');
clear all
close all
clc

dim = 14; 
Ncent = 256;

Ng = int2str(Ncent);
%%Training
actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');
scale_factor = 1;
shift = 0;

n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = int2str(1);

n_test = (n_peo-1)*n_actions;

load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-shift',  int2str(shift));
dim_FV = 2*dim*Ncent;


for pe_ts= 1: n_peo
    X_train = zeros(dim_FV,n_test);
    labels_train = zeros(n_test,1);
    k =1;
    for pe_tr=1: n_peo
        if pe_tr~=pe_ts
            for act=1: n_actions
                 %show_you = strcat(all_people(pe_tr),  '_', actions(act));
                 %disp(show_you);
                name_load_FV = strcat( load_sub_path, '/FV_', all_people(pe_tr),'_',actions(act),'_sc', sc, '_Ng', Ng, '.txt');
                one_FV = load( char( name_load_FV ) );
                X_train(:,k) = one_FV;
                labels_train(k) = act;
                k=k+1;               
            end
        end
    end
    
%size(X_train);
data_train = X_train';
labels_train = labels_train;
model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1' ]);
save_svm_model =strcat( './svm_models/run_person', int2str(pe_ts), '.mat')
save(save_svm_model, 'model');
    
    
end




