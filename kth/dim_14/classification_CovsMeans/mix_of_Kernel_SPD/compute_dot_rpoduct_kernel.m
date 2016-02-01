function [K] = compute_dot_rpoduct_kernel(X_1,X_2)

K = zeros(size(X_1,3),size(X_2,3));
%size(X_1)
%size(X_2)


for x_1 = 1: size(X_1,3)
    for x_2 = 1:size(X_2,3)
          K(x_1,x_2) = X_1(:,:,x_1)'*X_2(:,:,x_2);
          %K(x_1,x_2)
          %pause
    end
end