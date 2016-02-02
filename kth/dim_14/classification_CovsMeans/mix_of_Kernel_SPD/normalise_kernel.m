function Kc = normalise_kernel(K)

%function Kc = normalise(K)
%
% Normalises the kernel K
%
%INPUTS
% K = the non-normalised kernel K
%
%OUTPUTS
% Kc = the normalised kernel
%
%
%For more info, see www.kernel-methods.net


% original kernel matrix stored in variable K
% output uses the same variable K
% D is a diagonal matrix storing the inverse of the norms

%Normalisation 1
%D = diag(1./sqrt(diag(K)));
%Kc = D * K * D;

%Normalisation 2: 
%CLUSTERING ON GRASSMANN MANIFOLDS VIA KERNEL EMBEDDING
%WITH APPLICATION TO ACTION ANALYSIS
D = diag( sum(K,2) );
D = sqrt(D);
%D_inv  = inv(D_sqrt);

Kc = D\K/D_inv;

