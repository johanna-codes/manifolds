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
l = legend('KTH $_{m=9}$','UCF $_{m=3}$', 'UT $_{m=2}$', 'Location', 'northwest','orientation','vertical');
set(l,'Interpreter','Latex');

ylabel('accuraccy (%)','FontSize',20)
xlabel('Parameter $d$ for $K_{proj}^{poly}$','FontSize',20, 'Interpreter','Latex')
set(gca,'FontSize',20); 

%ylabel('accuraccy (%)','FontSize',20)
%xlabel('subspace order (m)','FontSize',20)

xlim([1 14])
ylim([0 70])

width = 8;     % Width in inches
height = 7;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
grid

%home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/ProjPoly_kth_ucf_ut','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v13/images/experiments/ProjPoly_kth_ucf_ut','-depsc2','-r300');


