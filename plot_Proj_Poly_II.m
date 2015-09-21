%From  plot_Proj_Poly_I.m I got the best m(subspace order) per dataset

%KTH***********************************************
load('./kth/dim_14/kernels/projPoly_all_p_performance.mat');
m = 9;
best_kth =  test_acc(m,:);

%UCF***********************************************
load('./ucf_sports/dim_14/classification/3-kernels/projPoly_all_p_performance.mat');
m = 3;
best_ucf =  test_acc(m,:);

%UT
load('./ut_tower/classification/3-kernels/projPoly_all_p_performance.mat');
m = 2;
best_ut =  test_acc(m,:);

%Plot************************
figure()
plot(d,best_kth, '-gd', 'LineWidth',3,'MarkerSize',10);
hold on
plot(d,best_ucf, '--ro', 'LineWidth',3,'MarkerSize',10);
plot(d,best_ut, '-.bx', 'LineWidth',3,'MarkerSize',10);
%title('Projection Metric','FontSize',20);
legend('KTH','UCF', 'UT Tower', 'Location', 'northwest','orientation','horizontal');
ylabel('accuraccy (%)','FontSize',20)
xlabel('subspace order (m)','FontSize',20)
set(gca,'FontSize',20); 
xlim([0 14])
ylim([0 70])

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

print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/ProjPoly_kth_ucf_ut','-dpng','-r300');

