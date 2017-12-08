function G = CholDot(A)
% G = CholDot(A)
% Cholesky factorization of a symmetric and positive definite matrix A.
% G is lower triangular so A = G*G’.
[n,n] = size(A); G = zeros(n,n);
for i=1:n
  % Compute G(i,1:i)
  for j=1:i
    if j==1
      s = A(j,i);
    else
      s = A(j,i) - G(j,1:j-1)*G(i,1:j-1)';
    end
    if j<i
      G(i,j) = s/G(j,j);
    else
      G(i,i) = sqrt(s);
    end
  end
end
