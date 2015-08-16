clear all
close all
clc

%% NN: Projection Metric: UCF_sports

p14=1:14;
p12=1:12;


PM_dim14 = [57.0470   65.7718   56.3758   63.0872   61.7450   52.3490   43.6242   38.9262   51.6779   46.3087   38.2550   24.8322 39.5973    9.3960];
PM_dim12 = [58.3893   68.4564   59.7315   58.3893   49.6644   45.6376   46.3087   47.6510   53.6913   24.1611   46.9799  9.3960];

plot(p14,PM_dim14, '-md', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p12,PM_dim12, '--ko', 'LineWidth',3,'MarkerSize',5);
legend('PM_{xy}','PM','Location', 'southwest');
title('UCF-sports dataset','FontSize',20);
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

ylim([0 75])
xlim([0 15])


%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v2/images/experiments_ucf/PM_ucf','-dpng','-r300');

%% Kernels: LED_POLY: KTH
figure

led_poly = [  ];

plot(led_poly, '-md','LineWidth',3,'MarkerSize',3)
title('UCF-sports','FontSize',20);
legend('LED POLY','Location', 'south');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);
%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v2/images/experiments_ucf/LED-POLY_ucf','-dpng','-r300');



  