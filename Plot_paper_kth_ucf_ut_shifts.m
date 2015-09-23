clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];

%% KTH d_led, K_led_poly, GMM & FV

LED_kth =         [75.3333   74.6667   81.3333   79.3333   78.6667   76.0000   74.6667   70.0000   74.0000   64.6667   66.6667 ];
LED_Poly_kth =    [52.0000   54.0000   64.6667   81.3333   86.6667   92.6667   87.3333   73.3333   57.3333   54.0000   53.3333 ];
GMM_kth =         [43.3333   56.6667   71.3333   84.6667   90.0000   86.6667   78.6667   78.6667   76.6667   68.6667   65.3333 ];
FV_kth =          [53.3333   62.6667   78.0000   90.6667   94.6667   96.6667   95.3333   92.6667   88.0000   84.6667   80.0000 ];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_shift,LED_kth, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_shift,LED_Poly_kth,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,GMM_kth, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_kth, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l = legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

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

%Nicta

print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_shifts_kth','-depsc2','-r300');
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v5/images/experiments/all_shifts_kth','-dpng','-r300');


%% UCF d_led, K_led_poly, GMM & FV
figure()
LED_ucf  =          [75.8389   75.1678   73.1544   71.1409   74.4966   76.5101   79.8658   69.7987   67.1141   65.7718   65.1007];
LED_Poly_ucf =      [44.9664   53.6913   52.3490   56.3758   69.1275   75.1678   55.7047   46.9799   43.6242   39.5973   35.5705];
GMM_ucf  =          [50.3356   54.3624   59.7315   69.1275   75.8389   80.5369   76.5101   65.7718   57.7181   51.6779   51.0067];
FV_ucf   =          [59.0604   63.7584   73.8255   79.1946   87.2483   88.5906   85.9060   77.1812   69.7987   56.3758   51.6779];

plot(vec_shift,LED_ucf, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_shift,LED_Poly_ucf,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,GMM_ucf, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_ucf, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l=legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

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

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/all_shifts_ucf','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_shifts_ucf','-depsc2','-r300');

%% UT_Tower d_led, K_led_poly, GMM & FV
figure()
LED_ut          = [79.6296   77.7778   83.3333   78.7037   75.0000   73.1481   74.0741   79.6296   81.4815   78.7037   81.4815];
LED_Poly_ut    =  [48.1481   62.9630   71.2963   75.9259   84.2593   83.3333   84.2593   82.4074   82.4074   75.9259   61.1111];
GMM_ut          = [60.1852   75.0000   86.1111   87.0370   90.7407   87.9630   87.9630   85.1852   82.4074   74.0741   61.1111];
FV_ut           = [61.6822   76.6355   85.9813   90.6542   91.5888   92.5234   93.4579   89.7196   88.7850   78.5047   60.7477];

plot(vec_shift,LED_ut, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_shift,LED_Poly_ut, '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,GMM_ut, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_ut, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l = legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 
grid

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
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/all_shifts_ucf','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_shifts_ut','-depsc2','-r300');





