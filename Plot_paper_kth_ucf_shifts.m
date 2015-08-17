clear all
close all
clc

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];

%% KTH GMM & FV

GMM_all_acc = [43.3333   56.6667   71.3333   84.6667   90.0000   86.6667   78.6667   78.6667   76.6667   68.6667   65.3333 ];
FV_all_acc =  [53.3333   62.6667   78.0000   90.6667   94.6667   96.6667   95.3333   92.6667   88.0000   84.6667   80.0000];

all = [GMM_all_acc; FV_all_acc ];
plot(vec_shift,all', '-d', 'LineWidth', 3,'MarkerSize', 10);
legend('GMM','FV','Location','southeast');
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

%Nicta
print('/home/johanna/latex-svn/wacv_2016_b/v3/images/experiments_kth/all_shifts_kth','-dpng','-r300');



%% KTH GMM & FV
figure()
%GMM_all_acc = [43.3333   56.6667   71.3333   84.6667   90.0000   86.6667   78.6667   78.6667   76.6667   68.6667   65.3333 ];
FV_all_acc   = [59.0604   63.7584   73.8255   79.1946   87.2483   88.5906   85.9060   77.1812   69.7987   56.3758    51.6779];

all = [FV_all_acc ];
plot(vec_shift,all', '-d', 'LineWidth', 3,'MarkerSize', 10);
legend('FV','Location','northeast');



