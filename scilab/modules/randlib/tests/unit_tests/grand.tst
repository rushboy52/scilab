// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- TEST WITH GRAPHIC -->

//---------------------Tests added by Sabine Gaüzère----------------------------


//Same tests as below but with loops in order to have more examples.
// test for grand
prec = 1

// test for exp 

for i=0.1:0.1:50,
  N=10000;A=i;
  Rdev=grand(1,N,'exp',A);
  RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
  PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
  P=1-exp(-RdevS/A);
//plot2d(RdevS,P,2,"000")
  if norm(P-PS) > prec then pause,end
end


// test for gamma 
for i=1:1:15,
  for j=1:1:15,
    N=10000;A=i;B=j;
    Rdev=grand(1,N,'gam',A,B); 
    RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
    PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
    [P]=cdfgam("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
    if norm(P-PS)> prec then pause,end
  end
end


// test for beta random deviate 
for i=1:1:20,
  for j=1:1:20,
    N=10000;A=i;B=j;
    Rdev=grand(1,N,'bet',A,B); 
    RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
    PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
    [P]=cdfbet("PQ",RdevS,1-RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
    if norm(P-PS)> prec then pause,end
  end
end

// test for poi 
for i=50:1:70,
  N=10000;A=i;
  Rdev=grand(1,N,'poi',A);
  RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
  PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
  [P]=cdfpoi("PQ",RdevS,A*ones(RdevS));
//plot2d(RdevS,P,2,"000")
// idem need an other test P is piecewize linear and PS 
// linear 
//if norm(P-PS) > prec then pause,end
end

//Tests to verify the compatibility with a matrix
  
N=100;A=1;B=3;
Rdev=grand(N,N,'bet',A,B); 
Rdev=matrix(Rdev,1,N^2);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:(N^2))'/(N^2);
[P]=cdfbet("PQ",RdevS,1-RdevS,A*ones(RdevS),B*ones(RdevS));
if norm(P-PS) > prec then pause,end
  
N=100;A=1;B=3;
Rdev=grand(N,N,'gam',A,B); 
Rdev=matrix(Rdev,1,N^2);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:(N^2))'/(N^2);
[P]=cdfgam("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
if norm(P-PS) > prec then pause,end
  
N=100;A=5;B=0.7;
Rdev=grand(N,N,'bin',A,B); 
Rdev=matrix(Rdev,1,N^2);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:(N^2))'/(N^2);
[P]=cdfbin("PQ",RdevS,A*ones(RdevS),B*ones(RdevS),(1-B)*ones(RdevS));
//if norm(P-PS) > prec then pause,end

N=100;A=50;
Rdev=grand(N,N,'poi',A); 
Rdev=matrix(Rdev,1,N^2);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:(N^2))'/(N^2);
[P]=cdfpoi("PQ",RdevS,A*ones(RdevS));
//if norm(P-PS) > prec then pause,end

N=100;A=2;
Rdev=grand(N,N,'exp',A); 
Rdev=matrix(Rdev,1,N^2);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:(N^2))'/(N^2);
P=1-exp(-RdevS/A);
if norm(P-PS) > prec then pause,end

//Tests with graphic

//Comparison of pseudo-random numbers following an exponential distribution 
//and the density of this distribution
//Parameter of the distribution which can be modified
lambda=1.6;
// Number of random variable generated
N=100000;
//Generation of a vector of numbers following an exponential distribution
X = grand(1,N,"exp",lambda);
clf();
//Discretisation of the abscisses in classes
classes = linspace(0,12,25);
//Draw in histogram
histplot(classes,X)
//Draw the density 
x=linspace(0,12,25);
y = (1/lambda)*exp(-(1/lambda)*x);
plot2d(x,y,3);
f=gcf();
delete(f);

//Comparison of pseudo-random numbers following a beta distribution 
//and the density of this distribution
//Parameters of the distribution which can be modified
A=1;B=3;
// Number of random variable generated
N=100000;
//Generation of a vector of numbers following a beta distribution
X = grand(1,N,"bet",A,B);
clf();
//Discretisation of the abscisses in classes
classes = linspace(0,1,50);
//Draw in histogram
histplot(classes,X)
//Draw the density 
x=linspace(0,1,50);
y = (1/(beta(A,B))).*(x^(A-1)).*((1-x)^(B-1)) ;
plot2d(x,y,2);
f=gcf();
delete(f);

//Comparison of pseudo-random numbers following a gamma distribution 
//and the density of this distribution
//Parameters of the distribution which can be modified
A=2;B=1;
// Number of random variable generated
N=100000;
//Generation of a vector of numbers following a gamma distribution
X = grand(1,N,"gam",A,B);
clf();
//Discretisation of the abscisses in classes
classes = linspace(0,2,50);
//Draw in histogram
histplot(classes,X)
//Draw the density 
x=linspace(0,2,50);
y = (B/(gamma(A))).*exp(-B*x).*(B*x)^(A-1);
plot2d(x,y,2);
f=gcf();
delete(f);


//Comparison of pseudo-random numbers following a binomial distribution 
//and the density of this distribution
//Parameters of the distribution which can be modified
n=50;p=0.3;
// Number of random variable generated
N=100000;
//Generation of a vector of numbers following a binomial distribution
X = grand(1,N,"bin",n,p);
clf();
//Discretisation of the abscisses in classes
classes = linspace(0,n,n+1);
//Draw in histogram
histplot(classes,X)
//Draw the density
x=linspace(0,n,n+1);
y = binomial(p,n);
plot2d(x,y,2);
f=gcf();
delete(f);

//Comparison of pseudo-random numbers following a poisson distribution 
//and the density of this distribution
//Parameters of the distribution which can be modified
mu=50;
// Number of random variable generated
N=100000;
//Generation of a vector of numbers following a poisson distribution
X = grand(1,N,"poi",mu);
clf();
//Discretisation of the abscisses in classes
classes = linspace(0,2*mu,101);
//Draw in histogram
histplot(classes,X)
//Draw the density
[x]=linspace(0,2*mu,101);
y = exp(-mu).*(mu^x)./factorial(x);
plot2d(x,y,2);
f=gcf();
delete(f);

//---------------------------------------------------------------------------

// test for beta random deviate 

N=10000;A=1;B=3;
Rdev=grand(1,N,'bet',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfbet("PQ",RdevS,1-RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for f 

N=10000;A=1;B=3;
Rdev=grand(1,N,'f',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdff("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for mul
 

// test for gamma 

N=10000;A=1;B=3;
Rdev=grand(1,N,'gam',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfgam("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for nor 

N=10000;A=1;B=2;
Rdev=grand(1,N,'nor',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfnor("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for unf 

N=10000;A=1;B=2;
Rdev=grand(1,N,'unf',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=RdevS-A;
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for uin ( a finir ) 

N=10000;A=1;B=10;
Rdev=grand(1,N,'uin',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=RdevS-A;
//plot2d(RdevS,P,2,"000")
//need an other test XXX
//if norm(P-PS) > prec then pause,end

// test for lgi 

N=10000;
Rdev=grand(1,N,'lgi');
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=RdevS-A;
//plot2d(RdevS,P,2,"000")
//XXXX need an other test 
//if norm(P-PS) > prec then pause,end


// test fo perm 

N=1000;
Mat=grand(N,'prm',[1:10]');
if sum([1:10]')/10 - sum(Mat,'c')/N > 0 then pause;end 

// test for nbn 

N=10000;A=5;B=0.7;
Rdev=grand(1,N,'nbn',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfnbn("PQ",RdevS,A*ones(RdevS),B*ones(RdevS),(1-B)*ones(RdevS));
//plot2d(RdevS,P,2,"000")
//XXXX need an other test 
//if norm(P-PS) > prec then pause,end


// test for bin 

N=10000;A=5;B=0.7;
Rdev=grand(1,N,'bin',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfbin("PQ",RdevS,A*ones(RdevS),B*ones(RdevS),(1-B)*ones(RdevS));
//plot2d(RdevS,P,2,"000")
//XXX need to change test 
//if norm(P-PS) > prec then pause,end

// test for mn 

// test for 'def'

N=10000;
Rdev=grand(1,N,'def');
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=RdevS;
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for nch or chn 

N=10000;A=5;B=4;
Rdev=grand(1,N,'nch',A,B); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfchn("PQ",RdevS,A*ones(RdevS),B*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for nf or fnc

N=10000;A=5;B=4;C=10;
Rdev=grand(1,N,'nf',A,B,C); 
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdffnc("PQ",RdevS,A*ones(RdevS),B*ones(RdevS),C*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for chi 

N=10000;A=5;
Rdev=grand(1,N,'chi',A);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfchi("PQ",RdevS,A*ones(RdevS));
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

// test for poi 

N=10000;A=50;
Rdev=grand(1,N,'poi',A);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
[P]=cdfpoi("PQ",RdevS,A*ones(RdevS));
//plot2d(RdevS,P,2,"000")
// idem need an other test P is piecewize linear and PS 
// linear 
//if norm(P-PS) > prec then pause,end


// test for exp 

N=10000;A=2;
Rdev=grand(1,N,'exp',A);
RdevS=gsort(Rdev);RdevS=RdevS($:-1:1)';
PS=(1:N)'/N;
//plot2d(RdevS,PS);
// theorical result 
P=1-exp(-RdevS/A);
//plot2d(RdevS,P,2,"000")
if norm(P-PS) > prec then pause,end

