function CRR = RSR(TrainSet,TestSet,Beta,SR_Lambda_input)


SPARSE_SOLVER = 'CVX'; %options are CVX and SPAMS

%Generate Kernel
galleryKernel = exp(-Beta*Stein_Divergence(TrainSet.X,TrainSet.X));
probeKernel = exp(-Beta*Stein_Divergence(TestSet.X,TrainSet.X))';


%Normalize dictionary
KD = galleryKernel./repmat(sqrt(sum(galleryKernel.^2)),[size(galleryKernel,1) 1]);

%
KX = probeKernel./repmat(sqrt(sum(probeKernel.^2)),[size(probeKernel,1) 1]);
L1 = size(probeKernel,2);
if (strcmpi(SPARSE_SOLVER,'CVX'))
    SR_Lambda = SR_Lambda_input;
    A = KD;
    Number_Visual_Codes = size(A,2);
    scX = zeros(Number_Visual_Codes,L1);
    for tmpC1=1:L1
        if (mod(tmpC1,10) == 1)
            %fprintf('Working on sample %d/%d\n',tmpC1,L1);
        end
        cvx_begin quiet
        variable tmp_Sparse_X(Number_Visual_Codes);
        minimize( -2*tmp_Sparse_X'*KX(:,tmpC1)+tmp_Sparse_X'*KD*tmp_Sparse_X+SR_Lambda*norm(tmp_Sparse_X,1));
        % if you are interested in positive sparse codes, uncomment the following two lines
        %             subject to
        %                 tmp_Sparse_X >= 0;
        cvx_end
        scX(:,tmpC1) = tmp_Sparse_X;
    end %endfor
elseif (strcmpi(SPARSE_SOLVER,'SPAMS'))
    SR_Lambda = 1e-2;
    [KD_U,KD_D,~] = svd(KD);
    A = diag(sqrt(diag(KD_D)))*KD_U';
    D_Inv = KD_U*diag(1./sqrt(diag(KD_D)));
    KX = D_Inv'*KX;
    scX = full(mexLasso(KX,A,struct('mode',2,'lambda',SR_Lambda,'lambda2',0)));
else
    error('Sparse solver is not defined. This version accepts CVX or SPAMS');
end


%% Classification
qLabel = Classify_SRC(A,TrainSet.y,scX,KX);



CRR = sum((qLabel - TestSet.y == 0))/L1;