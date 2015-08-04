function action_seq_names = list_video_sequences(path_dataset)


actions = importdata('actionNames.txt');
n_actions = size(actions,1);
k = 1;
action_seq_names=cell(150,3);
%load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

for act=1:n_actions
   
 ss_folders   =    strcat(path_dataset, actions(act), '/list_folders.txt');
 list_folders = fileread(char(ss_folders));
 text = textscan(list_folders,'%s');
 text =  text{1};
 n_folders = size(text,1);
 
 for i=1:n_folders
   action_seq_names(k,1) = actions(act); %Action
   action_seq_names(k,2) = text(i); %Video Sequence
   action_seq_names(k,3) ={int2str(act)}; %Action index   
   k=k+1;
     
 end
 
 

end

