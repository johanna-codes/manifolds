function Kc = normalise_kernel(K, type)

%default normalisation type 
%type = 0;

if nargin < 2
  type = 0;
end


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

%% Without normalisation
if (type == 0)
Kc = K;
end


%% Normalisation 1
% http://cs.du.edu/~mitchell/mario_books/Kernel_Methods_for_Pattern_Analysis_-_John_Shawe-Taylor_&_Nello_Christianini.pdf
%page113
if (type ==1)
D = diag(1./sqrt(diag(K)));
Kc = D * K * D;
end

%% Normalisation 2: 
%CLUSTERING ON GRASSMANN MANIFOLDS VIA KERNEL EMBEDDING
%WITH APPLICATION TO ACTION ANALYSIS
if (type ==2)

D = diag( sum(K,2) );
D = sqrt(D);
Kc = D\K/D;
end

