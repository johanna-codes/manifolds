function CRR = KGDL(trn, tst, Solver_Flag, SR_lambda,nAtoms,dict_options)


fprintf('Learning the Grassmannian dictionary\n');
D = grassmann_dictionary_learning(trn.X,nAtoms,dict_options);
[gSC_alpha_trn,~,~] = gsc_func(trn.X,D,SR_lambda,Solver_Flag);
[gSC_alpha_tst,~,~] = gsc_func(tst.X,D,SR_lambda,Solver_Flag);

% %Classification
%Ridge regression
nClasses = max(trn.y);
nPoints = length(trn.y);
L = zeros(nClasses,nPoints);
L(sub2ind([nClasses,nPoints], trn.y, 1:nPoints)) = 1;
zeta = 1e-1;    %regularization parameter for ridge regression
big_alpha = gSC_alpha_trn*gSC_alpha_trn' + zeta*eye(nAtoms);
big_v = L*gSC_alpha_trn';
W = big_v/big_alpha;
[~,y_hat] = max(W*gSC_alpha_tst);
CRR = sum(y_hat == tst.y)/length(y_hat);