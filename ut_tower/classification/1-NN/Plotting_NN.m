
clear all
close all
clc

dim =14;
%% Log Euclidean distance 
logEucl_est  =load('./results/nn_Log_Eucl_est_labels.dat');
real =load('./results/nn_Log_Eucl_real_labels.dat');

acc_logEucl = length(find(logEucl_est==real))/length(logEucl_est)*100;


%% Grassmann Projection Metric & Binnet-Cauchy
p=1:dim;
all_PM_est = zeros(dim,1);

for i=1:dim
    
  PM_est =  load( strcat('./results/grass_PM_est_labels_p', num2str(i), '.dat') );    
  acc_PM = length(find(PM_est==real))/length(PM_est)*100;
  all_PM_est(i) = acc_PM;
  

   
end
plot(p,all_PM_est, '-cd', 'LineWidth',3,'MarkerSize',5);
hold on
legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 
xlim([1 dim])

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/ucf/v1/images/varying_p_NN_dim14','-dpng','-r300');




acc_PM = max(all_PM_est);
acc_BC = max(all_BC_est);

%% BAR with all performance 

ACC = [acc_logEucl acc_SteinDiv acc_PM acc_BC];
figure
labels = {'Log_Eucl';'Stein_Div';'Projection';'Binet-Cauchy'};
xname = strtrim(cellstr(num2str(ACC'))') 
bar(ACC, 'b')
text(1:numel(ACC),ACC,xname,'horizontalalignment','center','verticalalignment','bottom') 
ylim([0 90])
set(gca,'XTickLabel',labels,'FontSize',15 );
ylabel('Recognition Accuracy (%)')
%xlabel('Metric Employed')

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/ucf/v1/images/NN_all_dim14','-dpng','-r300');


