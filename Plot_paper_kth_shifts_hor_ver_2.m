clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];


%% KTH. Horizontal Shift
figure 
LED_kth_hor =         [76.6667   78.6667   78.0000   79.3333   79.3333   76.0000   76.0000   72.0000   73.3333   70.0000   68.6667];
%PM_kth_hor  =         [66.6667   68.0000   63.3333   62.6667   66.0000   67.3333   64.6667   59.3333   56.0000   52.0000   57.3333];
LED_Poly_kth_hor =    [60.6667   64.0000   67.3333   78.6667   87.3333   92.0000   85.3333   68.0000   60.0000   58.0000   56.0000 ];
Proj_rbf_kth_hor =    [56.6667   56.6667   59.3333   66.6667   71.3333   76.0000   64.0000   55.3333   53.3333   51.3333   51.3333];
%GMM_kth_hor =         [70.6667   74.6667   81.3333   86.0000   88.0000   86.6667   84.6667   82.0000   77.3333   75.3333   72.6667];
FV_kth_hor =          [80.6667   84.0000   91.3333   95.3333   95.3333   96.6667   97.3333   96.6667   96.6667   94.6667   94.6667];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_shift,LED_kth_hor, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,PM_kth_hor, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,LED_Poly_kth_hor,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,Proj_rbf_kth_hor,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);
%plot(vec_shift,GMM_kth_hor, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_kth_hor, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('KTH','FontSize',20)
xlim([-25 25])
ylim([45 100])
%l = legend('$d_{spd}$','$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('hor shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

%To save
width = 8;     % Width in inches
height = 7;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/suplementary_material/images/experiments/hor_shift_kth','-depsc2','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/hor_shift_kth_2','-depsc2','-r300');


%% KTH. Vertical Shift
figure 
LED_kth_ver =         [75.3333   78.0000   82.6667   78.0000   77.3333   76.0000   76.6667   76.6667   76.6667   78.0000   82.6667];
%PM_kth_ver  =         [60.0000   62.6667   62.6667   69.3333   64.0000   67.3333   64.6667   64.6667   65.3333   69.3333   62.6667];
LED_Poly_kth_ver =    [76.6667   84.0000   87.3333   88.6667   89.3333   92.0000   86.6667   87.3333   88.0000   85.3333   85.3333];
Proj_rbf_kth_ver =    [61.3333   66.6667   64.0000   66.0000   69.3333   76.0000   74.0000   74.0000   73.3333   69.3333   68.6667];
%GMM_kth_ver =         [70.6667   74.6667   81.3333   86.0000   88.0000   86.6667   84.6667   82.0000   77.3333   75.3333   72.6667];
FV_kth_ver =          [67.3333   81.3333   86.6667   91.3333   96.0000   96.6667   97.3333   94.0000   90.6667   90.0000   85.3333];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_shift,LED_kth_ver, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,PM_kth_ver, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,LED_Poly_kth_ver,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,Proj_rbf_kth_ver,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);
%plot(vec_shift,GMM_kth_ver, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_kth_ver, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('KTH','FontSize',20)
xlim([-25 25])
ylim([45 100])
%l = legend('$d_{spd}$','$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('ver shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

%To save
width = 8;     % Width in inches
height = 7;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/suplementary_material/images/experiments/ver_shift_kth','-depsc2','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/ver_shift_kth_2','-depsc2','-r300');


