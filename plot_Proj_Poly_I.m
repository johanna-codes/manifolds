%KTH
figure()

%KTH***********************************************
load('./kth/dim_14/kernels/projPoly_all_p_performance.mat');
%all_p_mat = zeros(length(d), length(p));
all_p_mat = zeros(length(d), 3);
k=1;
for i=8:10
    all_p_mat(:,k) = test_acc(i,:);
    k = k+1; 
end
subplot(311)
plot(d,all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('ProjPoly: KTH','FontSize',20);
xlim([1, max(d)]);
ylim([40, 65]);

ylabel('(%)','FontSize',20)
xlabel('d','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=8', 'm=9', 'm=10', 'Location','northwest', 'orientation','horizontal');
grid

%UCF***********************************************
load('./ucf_sports/dim_14/classification/3-kernels/projPoly_all_p_performance.mat');
%all_p_mat = zeros(length(d), length(p));
all_p_mat = zeros(length(d), 3);
k=1;
%for i=1:length(p)
for i=2:4
    all_p_mat(:,k) = test_acc(i,:);
    k = k+1; 
end
subplot(312)
plot(d,all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('ProjPoly: UCF','FontSize',20);
xlim([1, max(d)]);
ylim([10, 70]);

ylabel('(%)','FontSize',20)
xlabel('d','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=4', 'Location','northwest', 'orientation','horizontal');
grid

%UT
load('./ut_tower/classification/3-kernels/projPoly_all_p_performance.mat');
%all_p_mat = zeros(length(d), length(p));
all_p_mat = zeros(length(d), 3);
k=1;
%for i=1:length(p)
for i=[2 3 8]
    all_p_mat(:,k) = test_acc(i,:);
    k = k+1; 
end


subplot(313)
plot(d,all_p_mat, '-d','LineWidth',3,'MarkerSize',3);
title('ProjPoly: UT','FontSize',20);
xlim([6, 14]);
%ylim([10, 70]);

ylabel('(%)','FontSize',20)
xlabel('d','FontSize',20)
set(gca,'FontSize',20);
legend( 'm=2', 'm=3', 'm=8', 'Location','northwest', 'orientation','horizontal');
grid



width = 6;     % Width in inches
height = 5;    % Height in inches

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);
grid

print('/media/johanna/HD1T/latex-svn/wacv_2016_b/v10/images/experiments/ProjPoly_kth_ucf_ut','-dpng','-r300');
