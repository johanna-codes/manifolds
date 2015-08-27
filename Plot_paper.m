clear all
close all
clc

%% NN: Projection Metric: KTH & UCF

PM_kth = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
PM_ucf = [57.0470   65.7718   56.3758   63.0872   61.7450   52.3490   43.6242   38.9262   51.6779   46.3087   38.2550   24.8322 39.5973    9.3960];


plot(PM_kth, '-gd', 'LineWidth',3,'MarkerSize',10);

hold on
plot(PM_ucf, '--ro', 'LineWidth',3,'MarkerSize',10);
%title('Projection Metric','FontSize',20);
legend('KTH','UCF', 'Location', 'southwest');
ylabel('accuraccy (%)','FontSize',20)
xlabel('subspace order (m)','FontSize',20)
set(gca,'FontSize',20); 

ylim([0 80])
xlim([1 14])

%To save
width = 6;     % Width in inches
height = 5;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
grid

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v7/images/experiments/PM_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/PM_kth', '-dpng','-r300');


%% Kernels: LED_POLY: KTH & UCF 
figure()

led_poly_kth = [86.6667   89.3333   89.3333   88.0000   88.6667   90.0000   90.6667   90.6667   91.3333   91.3333   92.0000   92.0000   91.3333   92.0000   92.0000   92.0000   92.6667   92.6667   92.6667   92.6667];
led_poly_ucf = [75.1678   72.4832   72.4832   71.8121   70.4698   70.4698   70.4698   70.4698   71.8121   71.8121   71.8121   71.8121   71.8121   72.4832   71.8121   71.8121   72.4832   73.1544   73.1544   73.1544];

plot(led_poly_kth(1:14), '-gd','LineWidth',3,'MarkerSize',7)
hold on
plot(led_poly_ucf(1:14), '--ro','LineWidth',3,'MarkerSize',7)
title('LED POLY','FontSize',20);
ylim([50 100])
xlim([1 14])
legend('KTH', 'UCF','Location', 'southeast');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v7/images/experiments/LED-POLY_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v7/images/experiments/LED-POLY_kth_ucf','-dpng','-r300');

%% Kernels: ProjPOLY: 
%KTH
figure()

%KTH***********************************************
load('./kth/dim_14/kernels/projPoly_all_p_performance.mat');
%all_p_mat = zeros(length(d), length(p));
all_p_mat = zeros(length(d), 3);
k=1;
for i=8:10
    all_p_mat(:,k) = test_acc(i,:);
    k = k+1; 
end
subplot(211)
plot(d,all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('ProjPoly: KTH','FontSize',20);
xlim([1, max(d)]);
ylim([40, 65]);

ylabel('(%)','FontSize',20)
xlabel('d','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=8', 'm=9', 'm=10', 'Location','northwest', 'orientation','horizontal');
grid

%UCF***********************************************
load('./ucf_sports/dim_14/classification/3-kernels/projPoly_all_p_performance.mat');
%all_p_mat = zeros(length(d), length(p));
all_p_mat = zeros(length(d), 3);
k=1;
%for i=1:length(p)
for i=2:4
    all_p_mat(:,k) = test_acc(i,:);
    k = k+1; 
end
subplot(212)
plot(d,all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('ProjPoly: UCF','FontSize',20);
xlim([1, max(d)]);
ylim([10, 70]);

ylabel('(%)','FontSize',20)
xlabel('d','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=4', 'Location','northwest', 'orientation','horizontal');
grid


print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v7/images/experiments/ProjPoly_kth_ucf','-dpng','-r300');

%% Kernels: Proj_RBF: 
%KTH 
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
ylim([0, 109]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=6', 'm=7', 'm=8', 'Location','northwest', 'orientation','horizontal');
grid

% UCF
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
subplot(212)
line1=plot(delta(15:31),all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('Proj-RBF: UCF','FontSize',20);
xlim([0, 16]);
%ylim([0, 90]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=4', 'Location','northwest', 'orientation','horizontal');
grid

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v7/images/experiments/Proj-RBF_kth_ucf','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/Proj-RBF_kth','-dpng','-r300');










