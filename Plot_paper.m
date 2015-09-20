clear all
close all
clc

%% NN: Projection Metric: KTH & UCF

PM_kth = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
PM_ucf = [57.0470   65.7718   56.3758   63.0872   61.7450   52.3490   43.6242   38.9262   51.6779   46.3087   38.2550   24.8322 39.5973    9.3960];
PM_ut  = [72.2222   75.0000   73.1481   76.8519   71.2963   52.7778   54.6296   23.1481   25.0000   23.1481   44.4444   32.4074   33.3333   11.1111 ];

plot(PM_kth, '-gd', 'LineWidth',3,'MarkerSize',10);

hold on
plot(PM_ucf, '--ro', 'LineWidth',3,'MarkerSize',10);
plot(PM_ut, '-.bx', 'LineWidth',3,'MarkerSize',10);
%title('Projection Metric','FontSize',20);
legend('KTH','UCF', 'UT Tower', 'Location', 'southwest','orientation','horizontal');
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
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/PM_kth_ucf_ut','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/PM_kth', '-dpng','-r300');


%% Kernels: LED_POLY: KTH & UCF & UT
figure()

led_poly_kth = [86.6667   89.3333   89.3333   88.0000   88.6667   90.0000   90.6667   90.6667   91.3333   91.3333   92.0000   92.0000   91.3333   92.0000   92.0000   92.0000   92.6667   92.6667   92.6667   92.6667];
led_poly_ucf = [75.1678   72.4832   72.4832   71.8121   70.4698   70.4698   70.4698   70.4698   71.8121   71.8121   71.8121   71.8121   71.8121   72.4832   71.8121   71.8121   72.4832   73.1544   73.1544   73.1544];
led_poly_ut  = [83.3333   87.9630   86.1111   85.1852   85.1852   85.1852   85.1852   85.1852   86.1111   85.1852   85.1852   84.2593   85.1852   85.1852 ];
    

plot(led_poly_kth(1:14), '-gd','LineWidth',3,'MarkerSize',10)
hold on
plot(led_poly_ucf(1:14), '--ro','LineWidth',3,'MarkerSize',10)
plot(led_poly_ut(1:14), '-.bx','LineWidth',3,'MarkerSize',10)

title('LED POLY','FontSize',20);
ylim([60 100])
xlim([1 14])
legend('KTH', 'UCF','UT Tower','Location', 'northwest','orientation','horizontal');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);

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
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/LED_POLY_kth_ucf_ut','-dpng','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v7/images/experiments/LED-POLY_kth_ucf','-dpng','-r300');

%% Kernels: LED_RBF: KTH & UCF & UT
figure

load('./kth/dim_14/kernels/LED_RBF_all_delta_performance.mat');
led_rbf_kth = test_acc;

load('./ucf_sports/dim_14/classification/3-kernels/results/LED_RBF_all_delta_performance.mat')
led_rbf_ucf = test_acc;

load('ut_tower/classification/3-kernels/LED_RBF_all_delta_performance.mat');
led_rbf_ut = test_acc;

plot(delta, led_rbf_kth, '-gd','LineWidth',3,'MarkerSize',10)
hold on
plot(delta, led_rbf_ucf, '--ro','LineWidth',3,'MarkerSize',10)
plot(delta, led_rbf_ut, '-.bx','LineWidth',3,'MarkerSize',10)
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);

title('LED RBF','FontSize',20);
ylim([0 105])
%xlim([1 14])
legend('KTH', 'UCF','UT Tower','Location', 'northwest','orientation','horizontal');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);

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
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/LED_RBF_kth_ucf_ut','-dpng','-r300');
%Nicta


%% Kernels: ProjPOLY: 
%plot_Proj_Poly_I (Se ve muy fea) 
plot_Proj_Poly_II
%% Kernels: Proj_RBF: 
figure()
%plot_Proj_rbf_I(Se ve muy fea) 
plot_Proj_rbf_II
