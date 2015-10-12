%NICTA && Server
addpath('/home/johanna/toolbox/libsvm-3.20/matlab')
addpath('/home/johanna/toolbox/yael/matlab');
clear all
close all
clc

dim = 14;
Ncent = 256;

Ng = int2str(Ncent);

actions = importdata('actionNames.txt');
all_people = importdata('people_list.txt');


n_actions = size(actions,1);
n_peo =  size(all_people,1);
sc = int2str(1);

n_test = (n_peo-1)*n_actions;


dim_FV = 2*dim*Ncent;


%% Testing - Shifts
vec_shift = [ -25, -20, -15, -10, -5,  0, 5, 10, 15, 20, 25 ];

scale_factor = 1;
% for i=1:length(vec_shift)
%     show_you = strcat('Getting FVs for Horizontal', int2str( vec_shift(i) ) );
%     flag_shift = true; %For horizontal Shift
%     disp(show_you);
%     FV_kth_all_videos(Ncent, dim, scale_factor, vec_shift(i), flag_shift);
%     
%     show_you = strcat('Getting FVs for Vertical', int2str( vec_shift(i) ) );
%     flag_shift = false; %For horizontal Shift
%     disp(show_you);
%     FV_kth_all_videos(Ncent, dim, scale_factor, vec_shift(i),flag_shift);
% end
% 

flag_shift = false;

all_acc_shifts = zeros( length(vec_shift), 1);
for i=1:length(vec_shift)
    scale_factor = 1;
    shift = vec_shift(i);
    if (flag_shift==true)
        show_you = strcat('Horizontal Shift', int2str( vec_shift(i) ) );
        disp(show_you);
        load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-horshift',  int2str(shift));
     end
     
      if (flag_shift==false)
          show_you = strcat('Vertical Shift', int2str( vec_shift(i) ) );
          disp(show_you);
          load_sub_path =strcat('./FV_training/scale', int2str(scale_factor), '-horshift',  int2str(shift));
     end
     
    acc = 0;
    real_labels = zeros(n_peo*n_actions);
    est_labels  = zeros(n_peo*n_actions);
    
    j=1;
    for pe_ts= 1: n_peo
        
        load_svm_model = strcat( './svm_models/run_person', int2str(pe_ts), '.mat');
        load(load_svm_model); %loading model and X_train
        
        for act_ts = 1:n_actions
            
            %show_you = strcat(all_people(pe_ts),  '_', actions(act_ts));
            %disp(show_you);
            real_labels(j) = act_ts;
            
            name_load_FV = strcat( load_sub_path, '/FV_', all_people(pe_ts),'_',actions(act_ts), '_sc', sc, '_Ng', Ng, '.txt');
            one_FV = load( char( name_load_FV ) );
            X_test(:,1) = one_FV;
            
            [predicted_label, accuracy, prob_estimates] = svmpredict([act_ts], X_test', model, ['-b 1']);
            %predicted_label
            est_labels(j) = predicted_label;
            j=j+1;
            
            if predicted_label == act_ts
                acc = acc+1;
            end
            
        end
        
       
    end
    %[acc n_peo n_actions]
    acc = acc*100/(n_peo*n_actions)
    all_acc_shifts(i) = acc;
end

[vec_shift' all_acc_shifts]



