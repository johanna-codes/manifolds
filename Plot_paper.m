clear all
close all
clc

%% NN: Projection Metric: KTH & UCF

PM_kth = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
PM_ucf = [57.0470   65.7718   56.3758   63.0872   61.7450   52.3490   43.6242   38.9262   51.6779   46.3087   38.2550   24.8322 39.5973    9.3960];


plot(PM_kth, '-gd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(PM_ucf, '--ro', 'LineWidth',3,'MarkerSize',5);
title('Projection Metric','FontSize',20);
legend('KTH','UCF', 'south');
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

ylim([0 100])
xlim([0 15])

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/PM_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/PM_kth', '-dpng','-r300');


%% Kernels: LED_POLY: KTH & UCF(running)
figure()

led_poly_kth = [86.6667   89.3333   89.3333   88.0000   88.6667   90.0000   90.6667   90.6667   91.3333   91.3333   92.0000   92.0000 91.3333   92.0000   92.0000   92.0000   92.6667   92.6667   92.6667   92.6667];
%led_poly_ucf = [];

plot(led_poly_kth, '-gd','LineWidth',3,'MarkerSize',3)
title('LED POLY','FontSize',20);
legend('KTH dataset','Location', 'south');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);
%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/LED-POLY_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/LED-POLY_kth','-dpng','-r300');




%% Kernels: Proj_RBF: KTH & UCF
figure()


load('./kth/dim_14/kernels/projRBF_all_p_delta_performance.mat');
%Doing only for p=6:8
all_p_mat = zeros(length(delta(20:end)), 3);
k=1;
for i=6:8
  all_p_mat(:,k) = all_p{i}(20:end);
  k = k+1;
end

%max_projRBF = max(max(all_p_mat));
subplot(211)
line1=plot(delta(20:end),all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('Proj-RBF: KTH','FontSize',20);
xlim([5, max(delta)]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=6', 'm=7', 'm=8', 'Location','southwest', 'orientation','horizontal');




%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/Proj-RBF_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/Proj-RBF_kth','-dpng','-r300');






