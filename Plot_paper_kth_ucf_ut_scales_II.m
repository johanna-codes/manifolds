clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];

%% KTH d_led, K_led_poly, GMM & FV

LED_kth =         [74.0000   72.6667   75.3333   77.3333   79.3333   76.0000   80.0000   78.6667   80.6667   78.6667   78.0000 ];
%PM_kth =          [55.3333   58.0000   60.6667   66.6667   66.6667   67.3333   63.3333   66.0000   68.6667   68.6667   72.0000 ];   
LED_Poly_kth =    [55.3333   62.0000   68.6667   78.6667   84.6667   92.0000   82.6667   78.6667   79.3333   74.6667   72.6667 ];
Proj_rbf_kth =    [42.6667   52.0000   58.6667   58.0000   72.6667   76.0000   60.0000   60.0000   57.3333   55.3333   52.0000 ];
%GMM_kth =         [69.333    78.0000   79.3333   80.0000   85.3333   86.6667   88.6667   85.3333   84.0000   78.6667   76.0000 ];
FV_kth =          [86.0000   89.3333   95.3333   97.3333   96.6667   96.6667   96.0000   97.3333   92.6667   90.0000   84.6667 ];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_scale,LED_kth, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_scale,PM_kth, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,LED_Poly_kth,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,Proj_rbf_kth,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);
%plot(vec_scale,GMM_kth, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_kth, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('KTH','FontSize',20)
xlim([0.75 1.25])
ylim([40 105])
%l = legend('$d_{spd}$', '$d_{ls}$','$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l  = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'FV','Location','north', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('scale (%)','FontSize',20)
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

%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_scales_kth_2','-depsc2','-r300');
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_scales_kth','-depsc2','-r300');


%% UCF d_led, K_led_poly, GMM & FV
figure()
LED_ucf  =          [63.0872   68.4564   76.5101   77.1812   75.8389   76.5101   74.4966   72.4832   71.1409   71.1409   66.4430];
%PM_ucf   =          [56.3758   61.0738   56.3758   60.4027   59.7315   65.7718   59.7315   60.4027   57.7181   56.3758   50.3356];
LED_Poly_ucf =      [43.6242   44.2953   49.6644   57.0470   67.1141   75.1678   65.7718   62.4161   59.0604   54.3624   51.0067];
Proj_rbf_ucf =      [48.9933   48.9933   51.6779   55.7047   56.3758   61.7450   39.5973   42.2819   34.8993   30.2013   29.5302];
%GMM_ucf  =          [60.4027   63.7584   67.1141   69.1275   71.1409   80.5369   70.4698   64.4295   57.7181   48.9933   40.9396];
FV_ucf   =          [65.7718   70.4698   78.5235   81.2081   81.2081   88.5906   79.8658   77.8523   73.8255   67.7852   62.4161];

plot(vec_scale,LED_ucf, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_scale,PM_ucf, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,LED_Poly_ucf,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,Proj_rbf_ucf,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);
%plot(vec_scale,GMM_ucf, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_ucf, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('UCF-Sports','FontSize',20)
xlim([0.75 1.25])
ylim([29 105])
%l=legend('$d_{spd}$', '$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'FV','Location','north', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('scale (%)','FontSize',20)
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
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_scales_ucf','-depsc2','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_scales_ucf_2','-depsc2','-r300');

%% UT_Tower d_led, K_led_poly, GMM & FV
figure()
LED_ut          = [79.6296   76.8519   78.7037   77.7778   80.5556   73.1481   75.9259   81.4815   78.7037   79.6296   81.4815];
%PM_ut           = [73.1481   72.2222   75.0000   77.7778   76.8519   76.8519   80.5556   76.8519   75.0000   70.3704   74.0741];
LED_Poly_ut     = [75.0000   72.2222   73.1481   73.1481   73.1481   87.9630   68.5185   71.2963   69.4444   65.7407   58.3333];
Proj_rbf_ut     = [19.4444   20.3704   38.8889   49.0741   37.9630   79.6296   26.8519   28.7037   25.9259   20.3704   15.7407];
%GMM_ut          = [77.7778   78.7037   84.2593   87.0370   83.3333   87.9630   77.7778   70.3704   68.5185   63.8889   56.4815];
FV_ut           = [73.8318   82.2430   87.8505   89.7196   93.4579   92.5234   80.3738   75.7009   72.8972   64.4860   54.2056];

plot(vec_scale,LED_ut, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_scale,PM_ut, '-cv', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,LED_Poly_ut, '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,Proj_rbf_ut,  '-s', 'color',[102/255 0/255 51/255],  'LineWidth', 3,'MarkerSize', 10);
%plot(vec_scale,GMM_ut, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_ut, '-.m+', 'LineWidth', 3,'MarkerSize', 10);

title('UT-Tower','FontSize',20)
xlim([0.75 1.25])
ylim([15 105])
%l = legend('$d_{spd}$', '$d_{ls}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'GMM','FV','Location','north', 'orientation', 'horizontal');
l = legend('$d_{spd}$', '$K_{spd}^{poly}$', '$K_{ls}^{rbf}$', 'FV','Location','north', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('accuracy (%)','FontSize',20)
xlabel('scale (%) ','FontSize',20)
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
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v12/images/experiments/all_scales_ut','-depsc2','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v18/images/experiments/all_scales_ut_2','-depsc2','-r300');





