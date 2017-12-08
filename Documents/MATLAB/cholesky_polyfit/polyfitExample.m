% polyfit example using lib, inverse A, cholesky
% 07.12.17
clear 
clc
format short g

N = 7;
x=linspace(0,2*pi,N);
y=cos(x) + 0.05*randn(1,length(x));
M = 7-1;
% solve Ax=b
%       Ap=y, 
% A consists of inputs x, w/ length N and order M
p = polyfit(x,y,M); % polyn. curve fitting lib

x1 = linspace(0,2*pi,100);
y1 = polyval(p,x1);
stem(x,y,'r')
hold on
plot(x1,y1)

%% solve by inverse A
% set A(n,j) (j=column, n=rows)
A = [];
for j=0:(M)
  for n=0:(N-1)
    A(n+1,j+1) = x(n+1)^j;
  end
end
p1 = inverse(A) * y';
p1 = fliplr(p1')
% error check
ErrP = p1 - p;
assert(abs(mean(ErrP)) < 1e-6);

y2 = polyval(p1,x1);
plot(x1,y2,'--g')

%% solve by cholesky
% matrix A must be positiv definit!
G  = chol(A);
p_ = forwardSub(transpose(G), y', N);
p2 = backSub(G, p_, N)

y3 = polyval(p2,x1);
plot(x1,y3,'-.m')
hold off
