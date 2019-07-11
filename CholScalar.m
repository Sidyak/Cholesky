function G = CholScalar(A)
% G = CholScalar(A)
% Cholesky factorization of a symmetric and positive definite matrix A.
% G is lower triangular so A = G*G’.
[n,n] = size(A);
G = zeros(n,n);
for i=1:n
% Compute G(i,1:i)
  for j=1:i
    s = A(j,i);
    for k=1:j-1
      s = s - G(j,k)*G(i,k);
    end
    if j<i
      G(i,j) = s/G(j,j);
    else
      G(i,i) = sqrt(s);
    end
  end
end
