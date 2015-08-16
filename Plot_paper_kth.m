clear all
close all
clc

%% NN: Projection Metric: KTH

p14=1:14;
p12=1:12;

PM_dim14 = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
PM_dim12 = [56.0000   53.0000   51.0000   54.0000   74.0000   93.0000   96.0000   88.0000   80.0000   91.0000   74.0000   19.0000];


plot(p14,PM_dim14, '-gd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p12,PM_dim12, '--ro', 'LineWidth',3,'MarkerSize',5);
legend('PM_{xy}','PM','Location', 'southwest');

title('KTH dataset','FontSize',20);
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

ylim([0 100])
xlim([0 15])

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v2/images/experiments_kth/PM_kth','-dpng','-r300');


%% Kernels: LED_POLY: KTH
figure()

led_poly = [86.6667   89.3333   89.3333   88.0000   88.6667   90.0000   90.6667   90.6667   91.3333   91.3333   92.0000   92.0000 91.3333   92.0000   92.0000   92.0000   92.6667   92.6667   92.6667   92.6667];


plot(led_poly, '-rd','LineWidth',3,'MarkerSize',3)
title('KTH dataset','FontSize',20);
legend('LED POLY','Location', 'south');
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);
%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v2/images/experiments_kth/LED-POLY_kth','-dpng','-r300');


%% Kernels: Proj_RBF: KTH
figure()

load('/media/johanna/HD1T/codes/codes-git/manifolds/trunk/kth/dim_14/kernels/projRBF_all_p_delta_performance.mat');



%Doing only for p=6:8
all_p_mat = zeros(length(delta), 3);
k=1;
for i=6:8
  all_p_mat(:,k) = all_p{i};
  k = k+1;
end

max_projRBF = max(max(all_p_mat));
plot(delta,all_p_mat, '-d','LineWidth',3,'MarkerSize',3)
%min_max = [min(delta), max(delta)+1];
xlim([min(delta), max(delta)]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=6', 'm=7', 'm=8', 'Location','northwest');

%Home
print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v2/images/experiments_kth/Proj-RBFY_kth','-dpng','-r300');






