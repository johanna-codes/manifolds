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
%size(delta)

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
l = legend('KTH $_{m=7}$','UCF $_{m=4}$', 'UT $_{m=4}$', 'Location', 'northwest','orientation','vertical');
set(l,'Interpreter','Latex');

ylabel('accuraccy (%)','FontSize',20)
xlabel('Parameter $\delta$ for $K_{proj}^{rbf}$','FontSize',20, 'Interpreter','Latex')
set(gca,'FontSize',20); 
xlim([-14 20])
ylim([0 81])



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

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/ProjRbf_kth_ucf_ut','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v13/images/experiments/ProjRbf_kth_ucf_ut','-depsc2','-r300');

