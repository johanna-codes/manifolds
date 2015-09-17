function action_seq_names = list_video_sequences(path_dataset)


actions = importdata('actionNames.txt');
n_actions = size(actions,1);
k = 1;
action_seq_names=cell(108,3);
%load_sub_path =strcat(path, 'cov_matrices/kth-one-cov-mat-dim', int2str(dim), '/sc', int2str(sc), '/scale', int2str(scale_factor), '-shift', int2str(shift) );

for act=1:n_actions
   
  
 for seq=1:12
   action_seq_names(k,1) = actions(act); %Action
   action_seq_names(k,2) = {int2str(act)}; %Action ID
   action_seq_names(k,3) = {int2str(seq)}; %Video Sequence
   k=k+1;
     
 end
 
 

end

