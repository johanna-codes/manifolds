clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];

%% KTH d_led, K_led_poly, GMM & FV

LED_kth =         [75.3333   74.6667   81.3333   79.3333   78.6667   76.0000   74.6667   70.0000   74.0000   64.6667   66.6667 ];
%PM_kth  =         [59.3333   56.6667   55.3333   64.0000   66.0000   67.3333   58.6667   62.0000   60.0000   56.6667   49.3333 ];
LED_Poly_kth =    [52.0000   55.3333   64.0000   82.6667   87.3333   92.0000   86.0000   71.3333   57.3333   54.0000   52.6667 ];
Proj_rbf_kth =    [41.3333   48.6667   52.6667   66.0000   70.6667   76.0000   71.3333   66.6667   46.6667   47.3333   41.3333 ];
KSR_spd_kth =     [57.3333   62.0000   70.0000   81.3333   82.0000   80.0000   80.0000   71.3333   54.6667   54.6667   52.6667 ];

KSR_ls_kth =     [46.0000   52.6667   53.3333   64.0000   68.0000   74.0000   67.3333   68.6667   48.0000   38.6667   43.3333];

%GMM_kth =         [43.3333   56.6667   71.3333   84.6667   90.0000   86.6667   78.6667   78.6667   76.6667   68.6667   65.3333 ];
FV_kth =          [53.3333   62.6667   78.0000   90.6667   94.6667   96.6667   95.3333   92.6667   88.0000   84.6667   80.0000 ];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_shift,LED_kth, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,PM_kth, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,LED_Poly_kth,  '--o', 'color', [0 0.5 0] ,  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,Proj_rbf_kth,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);

plot(vec_shift, KSR_spd_kth,  '--xb', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift, KSR_ls_kth,   '--xr', 'LineWidth', 3,'MarkerSize', 10);


%plot(vec_shift,GMM_kth, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_kth, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('KTH','FontSize',20)
xlim([-25 25])
ylim([40 105])
%l = legend('$d_{spd}$','$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'KSR$_{spd}$','KSR$_{ls}$','FV','Location','north', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

%To save
width = 10;     % Width in inches
height = 9;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_shifts_kth_2','-depsc2','-r300');
print('/home/johanna/latex-svn/mlsda_2016/suplementary_material/v3/images/experiments/extra_all_shifts_kth_SR','-depsc2','-r300');

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_shifts_kth','-depsc2','-r300');


%% UCF d_led, K_led_poly, GMM & FV
figure()
LED_ucf  =          [75.8389   75.1678   73.1544   71.1409   74.4966   76.5101   79.8658   69.7987   67.1141   65.7718   65.1007];
%PM_ucf  =          [53.0201   57.0470   56.3758   57.0470   61.7450   65.7718   65.1007   57.0470   53.0201   52.3490   45.6376];
LED_Poly_ucf =      [44.9664   53.6913   52.3490   56.3758   69.1275   75.1678   55.7047   46.9799   43.6242   39.5973   35.5705];
Proj_rbf_ucf =      [31.5436   31.5436   32.8859   34.8993   38.9262   61.7450   32.8859   28.8591   26.8456   20.1342   18.1208];
KSR_spd_ucf =       [46.9799   48.9933   55.7047   62.4161   70.4698   76.5101   65.1007   48.9933   44.9664   40.2685   36.2416];
KSR_ls_ucf  =       [41.6107   47.6510   55.0336   58.3893   69.7987   73.1544   63.7584   53.0201   42.2819   36.2416   30.2013];
    

%GMM_ucf  =          [50.3356   54.3624   59.7315   69.1275   75.8389   80.5369   76.5101   65.7718   57.7181   51.6779   51.0067];
FV_ucf   =          [59.0604   63.7584   73.8255   79.1946   87.2483   88.5906   85.9060   77.1812   69.7987   56.3758   51.6779];

plot(vec_shift,LED_ucf, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,PM_ucf, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,LED_Poly_ucf,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,Proj_rbf_ucf,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);

plot(vec_shift, KSR_spd_ucf,  '--xb', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift, KSR_ls_ucf,  '--xr', 'LineWidth', 3,'MarkerSize', 10);

%plot(vec_shift,GMM_ucf, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_ucf, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('UCF-Sports','FontSize',20)

xlim([-25 25])
ylim([18 105])
%l=legend('$d_{spd}$', '$d_{ls}$', '$K_{led}^{spd}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
%l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'KSR$_{spd}$', 'FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'KSR$_{spd}$','KSR$_{ls}$','FV','Location','north', 'orientation', 'horizontal');

set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

%To save
width = 10;     % Width in inches
height = 9;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_shifts_ucf','-depsc2','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_shifts_ucf_2','-depsc2','-r300');
print('/home/johanna/latex-svn/mlsda_2016/suplementary_material/v3/images/experiments/extra_all_shifts_ucf_SR','-depsc2','-r300');


%% UT_Tower d_led, K_led_poly, GMM & FV
figure()
LED_ut           = [79.6296   77.7778   83.3333   78.7037   75.0000   73.1481   74.0741   79.6296   81.4815   78.7037   81.4815 ];
%PM_ut           = [61.1111   68.5185   67.5926   74.0741   80.5556   76.8519   75.0000   76.8519   78.7037   80.5556   74.0741 ];
LED_Poly_ut      = [48.1481   62.0370   70.3704   76.8519   87.9630   87.9630   87.0370   85.1852   82.4074   75.9259   59.2593 ];
Proj_rbf_ut      = [11.1111   12.9630   18.5185   35.1852   79.6296   79.6296   75.9259   50.0000   34.2593   29.6296   22.2222 ];

KSR_spd_ut       = [54.6296   66.6667   72.2222   77.7778   81.4815   81.4815   82.4074   77.7778   75.0000   70.3704   58.3333];
KSR_ls_ut       = [11.1111   13.8889   25.0000   39.8148   77.7778   78.7037   70.3704   61.1111   55.5556   29.6296   22.2222];
%GMM_ut          = [60.1852   75.0000   86.1111   87.0370   90.7407   87.9630   87.9630   85.1852   82.4074   74.0741   61.1111 ];
FV_ut            = [61.6822   76.6355   85.9813   90.6542   91.5888   92.5234   93.4579   89.7196   88.7850   78.5047   60.7477 ];

plot(vec_shift,LED_ut, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,PM_ut, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,LED_Poly_ut, '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,Proj_rbf_ut,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);

plot(vec_shift, KSR_spd_ut,  '--xb', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift, KSR_ls_ut,  '--xr', 'LineWidth', 3,'MarkerSize', 10);

%plot(vec_shift,GMM_ut, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_ut, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('UT-Tower','FontSize',20)
xlim([-25 25])
ylim([10 105])
%l = legend('$d_{spd}$', '$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
%l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'KSR$_{spd}$', 'FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'KSR$_{spd}$','KSR$_{ls}$','FV','Location','north', 'orientation', 'horizontal');

set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

%To save
width = 10;     % Width in inches
height = 9;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_shifts_ut','-depsc2','-r300');
%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_shifts_ut_2','-depsc2','-r300');
print('/home/johanna/latex-svn/mlsda_2016/suplementary_material/v3/images/experiments/extra_all_shifts_ut_SR','-depsc2','-r300');






