%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');
clear all
close all
clc
dbstop error;


dim = 14;


%prompt = 'Number of Gaussians? ';
%Ncent = input(prompt)
%Ncent = 16;

%prompt = 'Number of SPD per video? ';
%numSPD = input(prompt)


vec_Ncent  = [ 2 4 8 16 32 60 128 256];
vec_numSPD = [ 2 4 8 16 32 60 128 256];

dim_spdvec  = dim*( dim + 1 )/2;



actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = int2str(1);

n_test = (n_peo-1)*n_actions;



%% Get FV. Run Just once
scale_factor = 1;
shift =0;


for i=1:length(vec_numSPD)
    numSPD = vec_numSPD(i);
    FV_kth_all_videos_mltipleSPD(Ncent, numSPD, dim_spdvec, scale_factor, shift);
end



%% Training
scale_factor = 1;
shift = 0;
load_sub_path =strcat('./FV_training_mulSPD/scale', int2str(scale_factor), '-shift',  int2str(shift));

for i=1:length(vec_Ncent)
    Ncent = vec_Ncent(i);
    for j=1:length(vec_numSPD)
        numSPD = vec_numSPD(j);
        Ng = int2str(Ncent);
        dim_FV = 2*dim_spdvec*Ncent;


        
        for pe_ts= 1: n_peo
            X_train = zeros(dim_FV,n_test);
            labels_train = zeros(n_test,1);
            k =1;
            for pe_tr=1: n_peo
                if pe_tr~=pe_ts
                    for act=1: n_actions
                        %show_you = strcat(all_people(pe_tr),  '_', actions(act));
                        %disp(show_you);
                        
                        name_load_FV = strcat( load_sub_path, '/FV_', all_people(pe_tr),'_',actions(act),'_sc', sc, '_Ng', Ng,  '_numSPD_', num2str(numSPD), '.txt');
                        one_FV = load( char( name_load_FV ) );
                        X_train(:,k) = one_FV;
                        labels_train(k) = act;
                        k=k+1;
                    end
                end
            end
            
            %size(X_train);
            data_train = X_train';
            model = svmtrain(labels_train, data_train, ['-s 0 -t 0 -b 1 -q' ]);
            save_svm_model =strcat( './svm_models/run_person', int2str(pe_ts),  '_Ng', Ng,  '_numSPD_', num2str(numSPD), '.mat');
            save(save_svm_model, 'model');
            
        end
    end
end

%% Testing

scale_factor = 1;
shift = 0;

all_acc_Ncent_NumSPD= zeros(  length(vec_Ncent), length(vec_numSPD) );

for i=1:length(vec_Ncent)
    Ncent = vec_Ncent(i);
    for j=1:length(vec_numSPD)
        numSPD = vec_numSPD(j);
        
        load_sub_path =strcat('./FV_training_mulSPD/scale', num2str(scale_factor), '-shift',  int2str(shift));
        acc = 0;
        real_labels = zeros(n_peo*n_actions,1);
        est_labels  = zeros(n_peo*n_actions,1);
        
        j=1;
        for pe_ts= 1: n_peo
            
            load_svm_model = strcat( './svm_models/run_person', int2str(pe_ts),  '_Ng', Ng,  '_numSPD_', num2str(numSPD), '.mat');
            load(load_svm_model); %loading model and X_train
            
            for act_ts = 1:n_actions
                
                %show_you = strcat(all_people(pe_ts),  '_', actions(act_ts));
                %disp(show_you);
                real_labels(j) = act_ts;
                
                name_load_FV = strcat( load_sub_path, '/FV_', all_people(pe_ts),'_',actions(act_ts), '_sc', sc, '_Ng', Ng,  '_numSPD_', num2str(numSPD), '.txt');
                one_FV = load( char( name_load_FV ) );
                X_test(:,1) = one_FV;
                
                [predicted_label, accuracy, prob_estimates] = svmpredict([act_ts], X_test', model, ['-b 1 ']);
                %predicted_label
                est_labels(j) = predicted_label;
                j=j+1;
                
                if predicted_label == act_ts
                    acc = acc+1;
                end
                
            end
            
            %save_labels = strcat('./svm_results/scale', num2str(scale_factor), '-shift', int2str(shift),'.mat' );
            %save(save_labels, 'est_labels', 'real_labels');
            
        end
        %[acc n_peo n_actions]
        acc = acc*100/(n_peo*n_actions)
        all_acc_Ncent_NumSPD(i,j) = acc
    end
end


