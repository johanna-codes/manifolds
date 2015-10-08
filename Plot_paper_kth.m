%% Extra experiments
clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

%% NN: Projection Metric: KTH & UCF

PM_kth = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
BC_kth = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   17.3333];
plot(PM_kth, '-gd', 'LineWidth',3,'MarkerSize',10);

hold on
plot(BC_kth, '--ro', 'LineWidth',3,'MarkerSize',10);

title('KTH','FontSize',20);
legend('Projection Metric','Binet-Cauchy Metric', 'Location', 'southwest','orientation','vertical');
ylabel('accuraccy (%)','FontSize',20)
xlabel('$m$','FontSize',20, 'Interpreter','Latex')
set(gca,'FontSize',20); 

ylim([10 70])
xlim([1 14])

%To save
width = 7;     % Width in inches
height = 6;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
grid

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/PM_kth_ucf_ut','-depsc2','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/suplementary_material/images/experiments/PM_BC_kth','-depsc2','-r300');
