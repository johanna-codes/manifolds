function CovMean = get_emb_LogCov(cov_i, mean_i)

dim = length(mean_i);
THRESH = 0.000001;
CovMean = zeros(dim+1,dim+1);
cov_i_2 = cov_i + mean_i*mean_i';

CovMean( 1:dim, 1:dim) = cov_i_2;    
CovMean( 1:dim, dim + 1)  = mean_i;

CovMean( dim + 1, 1:dim) = mean_i';
CovMean( dim + 1, dim + 1) = 1;
 

CovMean = 0.5*(CovMean + CovMean');
[V,D] = eig(CovMean);

diag_D = diag(D);

  q1 = find( diag_D < THRESH );
  
  if (length(q1)> 0)
  
    for pos = 1: length(q1)
    diag_D( q1(pos) ) = THRESH;
    end
  end
  
  new_D =   diag(diag_D);    
  CovMean = V*new_D*V';  
    

  