function label = my_output_svm_multiclass_kth (dec_values) 


A = [1 1 1 1 1 2 2 2 2 3 3 3 4 4 5];
B = [2 3 4 5 6 3 4 5 6 4 5 6  5 6 6];
sig_dec = sign(dec_values); 
hist = zeros(6,1); 

for i=1:length(sig_dec)
    if (sig_dec(i)) ==1 
       hist(A(i)) = hist(A(i)) + 1;
    else
       hist(B(i)) = hist(B(i)) + 1;
    end
 end
[maxi label] = max(hist);



