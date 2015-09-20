%KTH**************************************************************** 
load('./kth/dim_14/kernels/projRBF_all_p_delta_performance.mat');
%Doing only for p=6:8
all_p_mat = zeros(length(delta(20:end)), 3);
k=1;
for i=6:8
  all_p_mat(:,k) = all_p{i}(20:end);
  k = k+1;
end

%max_projRBF = max(max(all_p_mat));
subplot(311)
line1=plot(delta(20:end),all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('Proj-RBF: KTH','FontSize',20);
xlim([5, max(delta)]);
ylim([0, 109]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=6', 'm=7', 'm=8', 'Location','northwest', 'orientation','horizontal');
grid

% UCF**************************************************************** 
%figure()
load('./ucf_sports/dim_14/classification/3-kernels/projRBF_all_p_delta_performance.mat');
%Doing only for p=6:8
%all_p_mat = zeros( length(delta), length(all_p) );
all_p_mat = zeros( length(delta(15:31)), 3 );
k=1;
for i=2:4
  all_p_mat(:,k) = all_p{i}(15:31);
  k = k+1;
end

%max_projRBF = max(max(all_p_mat));
subplot(312)
line1=plot(delta(15:31),all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('Proj-RBF: UCF','FontSize',20);
xlim([0, 16]);
%ylim([0, 90]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=4', 'Location','northwest', 'orientation','horizontal');
grid

% UT-Tower**************************************************************** 
load('./ut_tower/classification/3-kernels/projRBF_all_p_delta_performance.mat');
%Doing only for p=6:8
%all_p_mat = zeros( length(delta), length(all_p) );
all_p_mat = zeros( length(delta(8:18)), 3 );
k=1;


%for i=1:length(all_p)
for i=2:4
  all_p_mat(:,k) = all_p{i}(8:18);
  k = k+1;
end

subplot(313)
%plot (delta,all_p_mat,'-d','LineWidth',3,'MarkerSize',3);
plot (delta(8:18),all_p_mat,'-d','LineWidth',3,'MarkerSize',3);
%max_projRBF = max(max(all_p_mat));
%subplot(212)
%line1=plot(delta(15:31),all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('Proj-RBF: UT','FontSize',20);
xlim([0, 20]);
ylim([0, 100]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=4', 'Location','northwest', 'orientation','horizontal');
grid

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v7/images/experiments/Proj-RBF_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/Proj-RBF_kth','-dpng','-r300');










