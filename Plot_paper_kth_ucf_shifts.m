clear all
close all
clc

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];

%% KTH LED, GMM & FV

LED_kth =         [75.3333   74.6667   81.3333   79.3333   78.6667   76.0000   74.6667   70.0000   74.0000   64.6667   66.6667 ];
LED_Poly_kth =    [52.0000   54.0000   64.6667   81.3333   86.6667   92.6667   87.3333   73.3333   57.3333   54.0000   53.3333 ];
GMM_kth =         [43.3333   56.6667   71.3333   84.6667   90.0000   86.6667   78.6667   78.6667   76.6667   68.6667   65.3333 ];
FV_kth =          [53.3333   62.6667   78.0000   90.6667   94.6667   96.6667   95.3333   92.6667   88.0000   84.6667   80.0000 ];

%all = [LED; LED_Poly; GMM_all_acc; FV_all_acc ];
plot(vec_shift,LED_kth, '-bh', 'LineWidth', 3,'MarkerSize', 10);
hold on
plot(vec_shift,LED_Poly_kth, '--go', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,GMM_kth, ':r>', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_kth, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
legend('LED', 'LED-Poly', 'GMM','FV','Location','southeast', 'orientation', 'horizontal');
ylabel('(%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 

%Nicta
%print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/all_shifts_kth','-dpng','-r300');
%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v5/images/experiments/all_shifts_kth','-dpng','-r300');


%% UCF GMM & FV
figure()
LED_ucf  = [75.8389   75.1678   73.1544   71.1409   74.4966   76.5101   79.8658   69.7987   67.1141   65.7718   65.1007];
GMM_ucf  = [50.3356   54.3624   59.7315   69.1275   75.8389   80.5369   76.5101   65.7718   57.7181   51.6779   51.0067];
FV_ucf   = [59.0604   63.7584   73.8255   79.1946   87.2483   88.5906   85.9060   77.1812   69.7987   56.3758   51.6779];

plot(vec_shift,LED_ucf, '-bh', 'LineWidth', 3,'MarkerSize', 10);
hold on
%plot(vec_shift,LED_Poly_ucf, '--go', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,GMM_ucf, ':r>', 'LineWidth', 3,'MarkerSize', 10);
plot(vec_shift,FV_ucf, '-.m+', 'LineWidth', 3,'MarkerSize', 10);


xlim([-25 25])
legend('LED', 'GMM','FV','Location','south', 'orientation', 'horizontal');
ylabel('(%)','FontSize',20)
xlabel('shift (%)','FontSize',20)
set(gca,'FontSize',20); 

%Home
%print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v4/images/experiments/all_shifts_ucf','-dpng','-r300');
%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v7/images/experiments/all_shifts_ucf','-dpng','-r300');





