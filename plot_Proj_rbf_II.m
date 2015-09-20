%From  plot_Proj_Poly_I.m I got the best m(subspace order) per dataset

%KTH***********************************************
load('./kth/dim_14/kernels/projRBF_all_p_delta_performance.mat');
m = 7;
best_kth =  all_p{m,:};
delta = delta( [1:2:length(delta)]);
best_kth_2 = best_kth(1:2:length(best_kth));
%size(delta)


%UCF***********************************************
load('./ucf_sports/dim_14/classification/3-kernels/projRBF_all_p_delta_performance.mat');
m = 4;
best_ucf =  all_p{m,:};
delta = delta( [1:2:length(delta)]);
best_ucf_2 = best_ucf(1:2:length(best_ucf));
size(delta)

%UT
load('./ut_tower/classification/3-kernels/projRBF_all_p_delta_performance.mat');
m = 4;
best_ut =  all_p{m,:};
%size(delta)



%Plot************************
plot(delta, best_kth_2, '-gd', 'LineWidth',3,'MarkerSize',10);
hold on
plot(delta,best_ucf_2, '--ro', 'LineWidth',3,'MarkerSize',10);
plot(delta,best_ut, '-.bx', 'LineWidth',3,'MarkerSize',10);
%title('Projection Metric','FontSize',20);
legend('KTH','UCF', 'UT Tower', 'Location', 'northwest','orientation','horizontal');
ylabel('accuraccy (%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20); 
xlim([-14 20])
ylim([0 100])



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

print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/ProjRbf_kth_ucf_ut','-dpng','-r300');


