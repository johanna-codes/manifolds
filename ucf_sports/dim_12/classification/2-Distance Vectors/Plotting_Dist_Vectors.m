
clear all
close all
clc

dim =12;
real = load( './svm_results/real_labels.dat');

%% Grassmann Projection Metric & Binnet-Cauchy
p=1:dim;
all_PM_est = zeros(dim,1);
all_BC_est = zeros(dim,1);
for i=1:dim
    
  PM_est =  load( strcat('./svm_results/GrassPM_scale1-shift0-p', num2str(i), '_est_labels.dat') );    
  acc_PM = length(find(PM_est==real))/length(PM_est)*100;
  all_PM_est(i) = acc_PM;
  
  BC_est =  load( strcat('./svm_results/GrassBC_scale1-shift0-p', num2str(i), '_est_labels.dat') );    
  acc_BC = length(find(BC_est==real))/length(BC_est)*100;
  all_BC_est(i) = acc_BC;
   
end
plot(p,all_PM_est, '-cd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p,all_BC_est, '-mo', 'LineWidth',3,'MarkerSize',5);
legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 
xlim([1 dim])

%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/ucf/v1/images/varying_p_dist_vect_dim12','-dpng','-r300');




acc_PM = max(all_PM_est);
acc_BC = max(all_BC_est);

%% BAR with all performance 

% ACC = [acc_PM acc_BC];
% figure
% labels = {'Log_Eucl';'Stein_Div';'Projection';'Binet-Cauchy'};
% xname = strtrim(cellstr(num2str(ACC'))') 
% bar(ACC, 'g')
% text(1:numel(ACC),ACC,xname,'horizontalalignment','center','verticalalignment','bottom') 
% ylim([0 110])
% set(gca,'XTickLabel',labels,'FontSize',15 );
% ylabel('Recognition Accuracy (%)')
% %xlabel('Metric Employed')
% 
% %Home
% print('/media/johanna/HD1T/latex-svn/study-paper-report/ucf/v1/images/dist_vect_all_dim14','-dpng','-r300');
% 
% 
