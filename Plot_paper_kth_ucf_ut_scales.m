clear all
close all
clc
%To save in eps depsc2
%To save in png dpng

vec_scale = [0.75 0.80 0.85 0.90 0.95 1 1.05 1.10  1.15 1.20 1.25];

%% KTH d_led, K_led_poly, GMM & FV

LED_kth =         [74.0000   72.6667   75.3333   77.3333   79.3333   76.0000   80.0000   78.6667   80.6667   78.6667   78.0000 ];
LED_Poly_kth =    [ ];
GMM_kth =         [ ];
FV_kth =          [ ];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_scale,LED_kth, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_scale,LED_Poly_kth,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,GMM_kth, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_kth, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l = legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('Scale (%)','FontSize',20)
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

print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_scales_kth','-depsc2','-r300');
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v5/images/experiments/all_scales_kth','-dpng','-r300');


%% UCF d_led, K_led_poly, GMM & FV
figure()
LED_ucf  =          [];
LED_Poly_ucf =      [];
GMM_ucf  =          [];
FV_ucf   =          [];

plot(vec_scale,LED_ucf, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_scale,LED_Poly_ucf,  '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,GMM_ucf, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_ucf, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l=legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('Scale (%)','FontSize',20)
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
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/all_scales_ucf','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_scales_ucf','-depsc2','-r300');

%% UT_Tower d_led, K_led_poly, GMM & FV
figure()
LED_ut          = [];
LED_Poly_ut    =  [];
GMM_ut          = [];
FV_ut           = [];

plot(vec_scale,LED_ut, '-kh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_scale,LED_Poly_ut, '--o', 'color', [0 0.5 0],  'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,GMM_ut, ':>', 'color', [1 0.5 0], 'LineWidth', 3,'MarkerSize', 10);
plot(vec_scale,FV_ut, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
ylim([30 100])
l = legend('$d_{LED}$', '$K_{LED}^{Poly}$', 'GMM','FV','Location','south', 'orientation', 'horizontal');
set(l,'Interpreter','Latex');
ylabel('(%)','FontSize',20)
xlabel('Scale ','FontSize',20)
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
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/all_scales_ucf','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v11/images/experiments/all_scales_ut','-depsc2','-r300');





