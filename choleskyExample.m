% cholesky decomposition example
% solve Ax=b via cholesky decomposition and forward/back substitution
% 07.12.17
clear
clc

A = [4 -10 2 ; -10 34 -17 ; 2 -17 18]

G = chol(A); % cholesky by lib
Gs = transpose(CholScalar(A));
Gd = transpose(CholDot(A));
assert(G==Gd);
assert(Gs==G);
assert(Gs==Gd);
A_= transpose(G) * G;
assert(A_==A);

b = [-10 7 22]'
x = inverse(A) * b

y = forwardSub(transpose(G),b,size(b));
x1 = backSub(G,y,size(y))

