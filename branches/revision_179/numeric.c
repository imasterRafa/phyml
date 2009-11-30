/*

PhyML:  a program that  computes maximum likelihood phylogenies from
DNA or AA homologous sequences.

Copyright (C) Stephane Guindon. Oct 2003 onward.

All parts of the source except where indicated are distributed under
the GNU public licence. See http://www.opensource.org for details.

*/

#include "utilities.h"
#include "free.h"
#include "lk.h"
#include "optimiz.h"
#include "models.h"
#include "eigen.h"
#include "numeric.h"

/*********************************************************/
/* RANDOM VARIATES GENERATORS */
/*********************************************************/

double Uni()
{
  double r; 
  r=rand();
  r/=RAND_MAX;
  return r;
}

/*********************************************************************/

int Rand_Int(int min, int max)
{
/*   phydbl u;   */
/*   u = (phydbl)rand(); */
/*   u /=  (RAND_MAX); */
/*   u *= (max - min + 1); */
/*   u += min; */
/*   return (int)floor(u); */

  int u;
  u = rand();
  return (u%(max+1-min)+min);

}

/*********************************************************/


/********************* random Gamma generator ************************
* Properties:
* (1) X = Gamma(alpha,lambda) = Gamma(alpha,1)/lambda
* (2) X1 = Gamma(alpha1,1), X2 = Gamma(alpha2,1) independent
*     then X = X1+X2 = Gamma(alpha1+alpha2,1)
* (3) alpha = k = integer then
*     X = Gamma(k,1) = Erlang(k,1) = -sum(log(Ui)) = -log(prod(Ui))
*     where U1,...Uk iid uniform(0,1)
*
* Decompose alpha = k+delta with k = [alpha], and 0<delta<1
* Apply (3) for Gamma(k,1)
* Apply Ahrens-Dieter algorithm for Gamma(delta,1)
*/
 
double Ahrensdietergamma(double alpha)
{
  double x = 0.;

  if (alpha>0.) 
    {
      double y = 0.;
      double b = (alpha+exp(1.))/exp(1.);
      double p = 1./alpha;
      int go = 0;
      while (go==0) 
	{
	  double u = Uni();
	  double w = Uni();
	  double v = b*u;
	  if (v<=1.) 
	    {
	      x = pow(v,p);
	      y = exp(-x);
	    }
	  else 
	    {
	      x = -log(p*(b-v));
	      y = pow(x,alpha-1.);
	    }
	  go = (w<y); // x is accepted when go=1
	}
    }
  return x;
}

/*********************************************************/

double Rgamma(double shape, double scale)
{
  int i;
  double x1 = 0.;
  double delta = shape;
  if (shape>=1.) 
    {
      int k = (int) floor(shape);
      delta = shape - k;
      double u = 1.;
      for (i=0; i<k; i++)
	u *= Uni();
      x1 = -log(u);
    }
  double x2 = Ahrensdietergamma(delta);
  return (x1 + x2)*scale;
}

/*********************************************************/

double Rexp(double lambda)
{
  return -log(Uni()+1.E-30)/lambda;
}

/*********************************************************/

phydbl Rnorm(phydbl mean, phydbl sd)
{
  /* Box-Muller transformation */
  phydbl u1, u2;

  u1=(phydbl)rand()/(RAND_MAX);
  u2=(phydbl)rand()/(RAND_MAX);
 
  u1 = sqrt(-2*log(u1))*cos(2*PI*u2);

  return u1*sd+mean;
}

/*********************************************************/

phydbl *Rnorm_Multid(phydbl *mu, phydbl *cov, int dim)
{
  phydbl *L,*x,*y;
  int i,j;
  
  x = (phydbl *)mCalloc(dim,sizeof(phydbl));
  y = (phydbl *)mCalloc(dim,sizeof(phydbl));


  L = (phydbl *)Cholesky_Decomp(cov,dim);


  For(i,dim) x[i]=Rnorm(0.0,1.0);
  For(i,dim) For(j,dim) y[i] += L[i*dim+j]*x[j];
  For(i,dim) y[i] += mu[i];

  Free(L);
  Free(x);

  return(y);
}

/*********************************************************/

phydbl Rnorm_Trunc(phydbl mean, phydbl sd, phydbl min, phydbl max)
{
  phydbl cdf_min, cdf_max, u;
  cdf_min = CDF_Normal(min,mean,sd);
  cdf_max = CDF_Normal(max,mean,sd);  
  u = cdf_min + (cdf_max-cdf_min) * Uni();
  return(sd*PointNormal(u)+mean);
}

/*********************************************************/

phydbl *Rnorm_Multid_Trunc(phydbl *mean, phydbl *cov, phydbl *min, phydbl *max, int dim)
{
  int i,j;
  phydbl *L,*x, *u;
  phydbl up, low, rec;
  
  u = (phydbl *)mCalloc(dim,sizeof(dim)); 
  x = (phydbl *)mCalloc(dim,sizeof(dim));
 
  L = Cholesky_Decomp(cov,dim);
  

  low = (min[0]-mean[0])/L[0*dim+0];
  up  = (max[0]-mean[0])/L[0*dim+0];
  u[0] = Rnorm_Trunc(0.0,1.0,up,low);


  for(i=1;i<dim;i++)
    {
      rec = .0;
      For(j,i) rec += L[i*dim+j] * u[j];
      low  = (min[i]-mean[i]-rec)/L[i*dim+i];
      up   = (max[i]-mean[i]-rec)/L[i*dim+i];
      u[i] = Rnorm_Trunc(0.0,1.0,up,low);
    }

  x = Matrix_Mult(L,u,dim,dim,dim,1);

/*   printf(">>>\n"); */
/*   For(i,dim) */
/*     { */
/*       For(j,dim) */
/* 	{ */
/* 	  printf("%10lf ",L[i*dim+j]); */
/* 	} */
/*       printf("\n"); */
/*     } */
/*   printf("\n"); */

/*   For(i,dim) printf("%f ",u[i]); */
/*   printf("\n"); */

  
/*   printf("\n"); */
/*   For(i,dim) */
/*     { */
/*       For(j,dim) */
/* 	{ */
/* 	  printf("%10lf ",x[i*dim+j]); */
/* 	} */
/*       printf("\n"); */
/*     } */
/*   printf("<<<\n"); */

  For(i,dim) x[i] += mean[i];

  Free(L);
  Free(u);
  
  return x;
}

/*********************************************************/
/* DENSITIES / PROBA */
/*********************************************************/

phydbl Dnorm_Moments(phydbl x, phydbl mean, phydbl var)
{
  phydbl dens,sd,pi;

  pi = 3.141593;
  sd = sqrt(var);

  dens = 1./(sqrt(2*pi)*sd)*exp(-((x-mean)*(x-mean)/(2.*sd*sd)));

  return dens;
}

/*********************************************************/

phydbl Dnorm(phydbl x, phydbl mean, phydbl sd)
{
  phydbl dens,pi;

  pi = 3.141593;

  dens = 1./(sqrt(2*pi)*sd)*exp(-((x-mean)*(x-mean)/(2.*sd*sd)));

  return dens;
}

/*********************************************************/

phydbl Pbinom(int N, int ni, phydbl p)
{
  return Bico(N,ni)*pow(p,ni)*pow(1-p,N-ni);
}

/*********************************************************/

phydbl Bivariate_Normal_Density(phydbl x, phydbl y, phydbl mux, phydbl muy, phydbl sdx, phydbl sdy, phydbl rho)
{
  phydbl cx, cy;
  phydbl pi;
  phydbl dens;
  phydbl rho2;

  pi = 3.141593;

  cx = x - mux;
  cy = y - muy;

  rho2 = rho*rho;

  dens = 1./(2*pi*sdx*sdy*sqrt(1.-rho2));
  dens *= exp((-1./(2.*(1.-rho2)))*(cx*cx/(sdx*sdx)+cy*cy/(sdy*sdy)+2*rho*cx*cy/(sdx*sdy)));
	      
  return dens;
}

/*********************************************************/

phydbl Dgamma_Moments(phydbl x, phydbl mean, phydbl var)
{
  phydbl shape, scale;

  if(var  < 1.E-20) 
    {
/*       var  = 1.E-20;  */
      PhyML_Printf("\n. var=%f mean=%f",var,mean);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Exit("\n");      
    }

  if(mean < 1.E-20) 
    { 
/*       mean = 1.E-20;  */
      PhyML_Printf("\n. var=%f mean=%f",var,mean);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Exit("\n");
    }


  shape = mean * mean / var;
  scale = var / mean;
  

  return(Dgamma(x,shape,scale));
}

/*********************************************************/

phydbl Dgamma(phydbl x, phydbl shape, phydbl scale)
{
  phydbl v;

  if(x == INFINITY) 
    {
      PhyML_Printf("\n. WARNING: huge value of x -> x = %G",x);
      x = 1.E+10;
    }

  if(x < 1.E-10)
    {
      if(x < 0.0) return 0.0;
      else
	{
	  PhyML_Printf("\n. WARNING: small value of x -> x = %G",x);
	  x = 1.E-10;
	}
    }


  if(scale < 0.0 || shape < 0.0)
    {
      PhyML_Printf("\n. scale=%f shape=%f",scale,shape);
      Exit("\n");
    }


  v = (shape-1.) * log(x) - shape * log(scale) - x / scale - LnGamma(shape);


  if(v < 500.)
    {
      v = exp(v);
    }
  else
    {
      PhyML_Printf("\n. WARNING v=%f x=%f shape=%f scale=%f",v,x,shape,scale);
      PhyML_Printf("\n. log(x) = %G LnGamma(shape)=%G",log(x),LnGamma(shape));
      Exit("\n");
    }

	 
  return v;
}

/*********************************************************/

phydbl Dexp(phydbl x, phydbl param)
{
  return param * exp(-param * x);
}

/*********************************************************/
phydbl Dpois(phydbl x, phydbl param)
{
  phydbl v;

  if(x < 0) 
    {
      printf("\n. x = %f",x);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }

  v = x * log(param) - param - LnGamma(x+1);

  if(v < 500)
    {
      v = exp(v);
    }
  else
    {
      PhyML_Printf("\n. WARNING v=%f x=%f param=%f",v,x,param);
      v = exp(500);
    }
  
/*   PhyML_Printf("\n. Poi %f %f (x=%f param=%f)", */
/* 	 v, */
/* 	 pow(param,x) * exp(-param) / exp(LnGamma(x+1)), */
/* 	 x,param); */
/*   return pow(param,x) * exp(-param) / exp(LnGamma(x+1)); */
  
  return v;
}

/*********************************************************/


/*********************************************************/
/* CDFs */
/*********************************************************/

phydbl CDF_Normal(phydbl x, phydbl mean, phydbl sd)
{
  const double b1 =  0.319381530;
  const double b2 = -0.356563782;
  const double b3 =  1.781477937;
  const double b4 = -1.821255978;
  const double b5 =  1.330274429;
  const double p  =  0.2316419;
  const double c  =  0.39894228;
  
  x = (x-mean)/sd;
  
  if(x >= 0.0) 
    {
      double t = 1.0 / ( 1.0 + p * x );
      return (1.0 - c * exp( -x * x / 2.0 ) * t *
	      ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }
  else 
    {
      double t = 1.0 / ( 1.0 - p * x );
      return ( c * exp( -x * x / 2.0 ) * t *
	       ( t *( t * ( t * ( t * b5 + b4 ) + b3 ) + b2 ) + b1 ));
    }
}

/*********************************************************/


phydbl CDF_Gamma(phydbl x, phydbl shape, phydbl scale)
{
  return IncompleteGamma(x/scale,shape,LnGamma(shape));
}

/*********************************************************/

phydbl CDF_Pois(phydbl x, phydbl param)
{
  /* Press et al. (1990) approximation of the CDF for the Poisson distribution */
  if(param < MDBL_MIN || x < 0.0) 
    {
      printf("\n. param = %G x=%G",param,x);
      PhyML_Printf("\n. Err in file %s at line %d\n",__FILE__,__LINE__);
      Warn_And_Exit("");
    }
  return IncompleteGamma(x,param,LnGamma(param));
}

/*********************************************************/

/*********************************************************/
/* Inverse CDFs */
/*********************************************************/

phydbl PointChi2 (phydbl prob, phydbl v)
{
/* returns z so that Prob{x<z}=prob where x is Chi2 distributed with df=v
   returns -1 if in error.   0.000002<prob<0.999998
   RATNEST FORTRAN by
       Best DJ & Roberts DE (1975) The percentage points of the
       Chi2 distribution.  Applied Statistics 24: 385-388.  (AS91)
   Converted into C by Ziheng Yang, Oct. 1993.
*/
   phydbl e=.5e-6, aa=.6931471805, p=prob, g;
   phydbl xx, c, ch, a=0,q=0,p1=0,p2=0,t=0,x=0,b=0,s1,s2,s3,s4,s5,s6;

   if (p<.000002 || p>.999998 || v<=0) return (-1);

   g = LnGamma (v/2);
   xx=v/2;   c=xx-1;
   if (v >= -1.24*(phydbl)log(p)) goto l1;

   ch=pow((p*xx*(phydbl)exp(g+xx*aa)), 1/xx);
   if (ch-e<0) return (ch);
   goto l4;
l1:
   if (v>.32) goto l3;
   ch=0.4;   a=(phydbl)log(1-p);
l2:
   q=ch;  p1=1+ch*(4.67+ch);  p2=ch*(6.73+ch*(6.66+ch));
   t=-0.5+(4.67+2*ch)/p1 - (6.73+ch*(13.32+3*ch))/p2;
   ch-=(1-(phydbl)exp(a+g+.5*ch+c*aa)*p2/p1)/t;
   if (fabs(q/ch-1)-.01 <= 0) goto l4;
   else                       goto l2;

l3:
   x=PointNormal (p);
   p1=0.222222/v;   ch=v*pow((x*sqrt(p1)+1-p1), 3.0);
   if (ch>2.2*v+6)  ch=-2*((phydbl)log(1-p)-c*(phydbl)log(.5*ch)+g);
l4:
   q=ch;   p1=.5*ch;
   if ((t=IncompleteGamma (p1, xx, g))<0) {
      PhyML_Printf ("\nerr IncompleteGamma");
      return (-1);
   }
   p2=p-t;
   t=p2*(phydbl)exp(xx*aa+g+p1-c*(phydbl)log(ch));
   b=t/ch;  a=0.5*t-b*c;

   s1=(210+a*(140+a*(105+a*(84+a*(70+60*a))))) / 420;
   s2=(420+a*(735+a*(966+a*(1141+1278*a))))/2520;
   s3=(210+a*(462+a*(707+932*a)))/2520;
   s4=(252+a*(672+1182*a)+c*(294+a*(889+1740*a)))/5040;
   s5=(84+264*a+c*(175+606*a))/2520;
   s6=(120+c*(346+127*c))/5040;
   ch+=t*(1+0.5*t*s1-b*c*(s1-b*(s2-b*(s3-b*(s4-b*(s5-b*s6))))));
   if (fabs(q/ch-1) > e) goto l4;

   return (ch);
}

/*********************************************************/

phydbl PointNormal (phydbl prob)
{
/* returns z so that Prob{x<z}=prob where x ~ N(0,1) and (1e-12)<prob<1-(1e-12)
   returns (-9999) if in error
   Odeh RE & Evans JO (1974) The percentage points of the normal distribution.
   Applied Statistics 22: 96-97 (AS70)

   Newer methods:
     Wichura MJ (1988) Algorithm AS 241: the percentage points of the
       normal distribution.  37: 477-484.
     Beasley JD & Springer SG  (1977).  Algorithm AS 111: the percentage
       points of the normal distribution.  26: 118-121.

*/
   phydbl a0=-.322232431088, a1=-1, a2=-.342242088547, a3=-.0204231210245;
   phydbl a4=-.453642210148e-4, b0=.0993484626060, b1=.588581570495;
   phydbl b2=.531103462366, b3=.103537752850, b4=.0038560700634;
   phydbl y, z=0, p=prob, p1;

   p1 = (p<0.5 ? p : 1-p);
   if (p1<1e-20) return (-INFINITY);

   y = sqrt ((phydbl)log(1/(p1*p1)));
   z = y + ((((y*a4+a3)*y+a2)*y+a1)*y+a0) / ((((y*b4+b3)*y+b2)*y+b1)*y+b0);
   return (p<0.5 ? -z : z);
}

/*********************************************************/
/* MISCs */
/*********************************************************/

/*********************************************************/

phydbl Bico(int n, int k)
{
  return floor(0.5+exp(Factln(n)-Factln(k)-Factln(n-k)));
}


/*********************************************************/

phydbl Factln(int n)
{
  static phydbl a[101];
  
  if (n < 0)    { Warn_And_Exit("\n. Err: negative factorial in routine FACTLN"); }
  if (n <= 1)     return 0.0;
  if (n <= 100)   return a[n] ? a[n] : (a[n]=Gammln(n+1.0));
  else return     Gammln(n+1.0);
}

/*********************************************************/

phydbl Gammln(phydbl xx)
{
  phydbl x,tmp,ser;
  static phydbl cof[6]={76.18009173,-86.50532033,24.01409822,
			-1.231739516,0.120858003e-2,-0.536382e-5};
  int j;
  
  x=xx-1.0;
  tmp=x+5.5;
  tmp -= (x+0.5)*(phydbl)log(tmp);
  ser=1.0;
  for (j=0;j<=5;j++) 
    {
      x += 1.0;
      ser += cof[j]/x;
    }
  return -tmp+(phydbl)log(2.50662827465*ser);
}

/*********************************************************/

/* void Plim_Binom(phydbl pH0, int N, phydbl *pinf, phydbl *psup) */
/* { */
/*   *pinf = pH0 - 1.64*sqrt(pH0*(1-pH0)/(phydbl)N); */
/*   if(*pinf < 0) *pinf = .0; */
/*   *psup = pH0 + 1.64*sqrt(pH0*(1-pH0)/(phydbl)N); */
/* } */

/*********************************************************/

phydbl LnGamma (phydbl alpha)
{
/* returns ln(gamma(alpha)) for alpha>0, accurate to 10 decimal places.
   Stirling's formula is used for the central polynomial part of the procedure.
   Pike MC & Hill ID (1966) Algorithm 291: Logarithm of the gamma function.
   Communications of the Association for Computing Machinery, 9:684
*/
   phydbl x=alpha, f=0, z;
   if (x<7) {
      f=1;  z=x-1;
      while (++z<7)  f*=z;
      x=z;   f=-(phydbl)log(f);
   }
   z = 1/(x*x);
   return  f + (x-0.5)*(phydbl)log(x) - x + .918938533204673
	  + (((-.000595238095238*z+.000793650793651)*z-.002777777777778)*z
	       +.083333333333333)/x;
}

/*********************************************************/

phydbl IncompleteGamma(phydbl x, phydbl alpha, phydbl ln_gamma_alpha)
{
/* returns the incomplete gamma ratio I(x,alpha) where x is the upper
	   limit of the integration and alpha is the shape parameter.
   returns (-1) if in error
   ln_gamma_alpha = ln(Gamma(alpha)), is almost redundant.
   (1) series expansion     if (alpha>x || x<=1)
   (2) continued fraction   otherwise
   RATNEST FORTRAN by
   Bhattacharjee GP (1970) The incomplete gamma integral.  Applied Statistics,
   19: 285-287 (AS32)
*/
   int i;
   phydbl p=alpha, g=ln_gamma_alpha;
   phydbl accurate=1e-8, overflow=1e30;
   phydbl factor, gin=0, rn=0, a=0,b=0,an=0,dif=0, term=0, pn[6];

   if (x==0) return (0);
   if (x<0 || p<=0) return (-1);

   factor=(phydbl)exp(p*(phydbl)log(x)-x-g);
   if (x>1 && x>=p) goto l30;
   /* (1) series expansion */
   gin=1;  term=1;  rn=p;
 l20:
   rn++;
   term*=x/rn;   gin+=term;

   if (term > accurate) goto l20;
   gin*=factor/p;
   goto l50;
 l30:
   /* (2) continued fraction */
   a=1-p;   b=a+x+1;  term=0;
   pn[0]=1;  pn[1]=x;  pn[2]=x+1;  pn[3]=x*b;
   gin=pn[2]/pn[3];
 l32:
   a++;  b+=2;  term++;   an=a*term;
   for (i=0; i<2; i++) pn[i+4]=b*pn[i+2]-an*pn[i];
   if (pn[5] == 0) goto l35;
   rn=pn[4]/pn[5];   dif=fabs(gin-rn);
   if (dif>accurate) goto l34;
   if (dif<=accurate*rn) goto l42;
 l34:
   gin=rn;
 l35:
   for (i=0; i<4; i++) pn[i]=pn[i+2];
   if (fabs(pn[4]) < overflow) goto l32;
   for (i=0; i<4; i++) pn[i]/=overflow;
   goto l32;
 l42:
   gin=1-factor*gin;

 l50:
   return (gin);
}


/*********************************************************/

int DiscreteGamma (phydbl freqK[], phydbl rK[],
		   phydbl alfa, phydbl beta, int K, int median)
{
  /* discretization of gamma distribution with equal proportions in each
     category
  */
   
  int i;
  phydbl gap05=1.0/(2.0*K), t, factor=alfa/beta*K, lnga1;

  if(K==1)
    {
      freqK[0] = 1.0;
      rK[0] = 1.0;
      return 0;
    }

   if (median) 
     {
       for (i=0; i<K; i++)     rK[i]=PointGamma((i*2.0+1)*gap05, alfa, beta);
       for (i=0,t=0; i<K; i++) t+=rK[i];
       for (i=0; i<K; i++)     rK[i]*=factor/t;
     }
   else {
      lnga1=LnGamma(alfa+1);
      for (i=0; i<K-1; i++)
	 freqK[i]=PointGamma((i+1.0)/K, alfa, beta);
      for (i=0; i<K-1; i++)
	 freqK[i]=IncompleteGamma(freqK[i]*beta, alfa+1, lnga1);
      rK[0] = freqK[0]*factor;
      rK[K-1] = (1-freqK[K-2])*factor;
      for (i=1; i<K-1; i++)  rK[i] = (freqK[i]-freqK[i-1])*factor;
   }
   for (i=0; i<K; i++) freqK[i]=1.0/K;
   return (0);
}

/*********************************************************/

/* Return log(n!) */

phydbl LnFact(int n)
{
  int i;
  phydbl res;

  res = 0;
  for(i=2;i<=n;i++) res += log(i);
  
  return(res);
}

/*********************************************************/

int Choose(int n, int k)
{
  phydbl accum;
  int i;

  if (k > n) return(0);
  if (k > n/2) k = n-k;
  if(!k) return(1);

  accum = 1.;
  for(i=1;i<k+1;i++) accum = accum * (n-k+i) / i;

  return((int)accum);
}

/*********************************************************/


phydbl *Covariance_Matrix(arbre *tree)
{
  phydbl *cov, *mean,var_min;
  int *ori_wght,*site_num;
  int dim,i,j,replicate,n_site,position,sample_size;

  sample_size = 1000;
  dim = 2*tree->n_otu-3;

  cov      = (phydbl *)mCalloc(dim*dim,sizeof(phydbl));
  mean     = (phydbl *)mCalloc(    dim,sizeof(phydbl));
  ori_wght = (int *)mCalloc(tree->data->crunch_len,sizeof(int));
  site_num = (int *)mCalloc(tree->data->init_len,sizeof(int));
  
  var_min = 1./pow(tree->data->init_len,2);

  For(i,tree->data->crunch_len) ori_wght[i] = tree->data->wght[i];

  n_site = 0;
  For(i,tree->data->crunch_len) For(j,tree->data->wght[i])
    {
      site_num[n_site] = i;
      n_site++;
    }

  		  
  tree->mod->s_opt->print = 0;
  For(replicate,sample_size)
    {
      For(i,2*tree->n_otu-3) tree->t_edges[i]->l = .1;

      For(i,tree->data->crunch_len) tree->data->wght[i] = 0;

      For(i,tree->data->init_len)
	{
	  position = Rand_Int(0,(int)(tree->data->init_len-1.0));
	  tree->data->wght[site_num[position]] += 1;
	}

      Round_Optimize(tree,tree->data,ROUND_MAX);
      
      For(i,2*tree->n_otu-3) For(j,2*tree->n_otu-3) cov[i*dim+j] += tree->t_edges[i]->l * tree->t_edges[j]->l;  
      For(i,2*tree->n_otu-3) mean[i] += tree->t_edges[i]->l;

      printf("."); fflush(NULL);
/*       printf("\n. %3d %12f %12f %12f [%12f %12f %12f] [%12f %12f %12f] [%12f %12f %12f]", */
/* 	     replicate, */
/*  	     cov[0]/(replicate+1)-mean[0]*mean[0]/pow(replicate+1,2), */
/*  	     cov[1]/(replicate+1)-mean[0]*mean[1]/pow(replicate+1,2), */
/*  	     cov[2]/(replicate+1)-mean[0]*mean[2]/pow(replicate+1,2), */
/* 	     tree->t_edges[0]->l, */
/* 	     tree->t_edges[1]->l, */
/* 	     tree->t_edges[2]->l, */
/* 	     mean[0]/(replicate+1), */
/* 	     mean[1]/(replicate+1), */
/* 	     mean[2]/(replicate+1), */
/* 	     cov[0]/(replicate+1), */
/* 	     cov[1]/(replicate+1), */
/* 	     cov[2]/(replicate+1)); */
   }

  For(i,2*tree->n_otu-3) mean[i] /= (phydbl)sample_size;
  
  For(i,2*tree->n_otu-3) For(j,2*tree->n_otu-3) cov[i*dim+j] /= (phydbl)sample_size;
  For(i,2*tree->n_otu-3) For(j,2*tree->n_otu-3) cov[i*dim+j] -= mean[i]*mean[j];
  For(i,2*tree->n_otu-3) if(cov[i*dim+i] < var_min) cov[i*dim+i] = var_min;
  

/*   printf("\n"); */
/*   For(i,2*tree->n_otu-3) printf("%f %f\n",mean[i],tree->t_edges[i]->l); */
/*   printf("\n"); */
/*   printf("\n"); */
/*   For(i,2*tree->n_otu-3) */
/*     { */
/*       For(j,2*tree->n_otu-3) */
/* 	{ */
/* 	  printf("%G\n",cov[i*dim+j]); */
/* 	} */
/*       printf("\n"); */
/*     } */

  For(i,tree->data->crunch_len) tree->data->wght[i] = ori_wght[i];

  Free(mean);
  Free(ori_wght);
  Free(site_num);

  return cov;
}

/*********************************************************/
/* Work out the (inverse of the) Hessian for the likelihood
   function. Only branch lengths are considered as variable */
phydbl *Hessian(arbre *tree)
{
  phydbl *hessian;
  phydbl *plus_plus, *minus_minus, *plus_zero, *minus_zero, *plus_minus, zero_zero;
  phydbl *ori_bl;
  int dim;
  int i,j;
  phydbl eps;
  phydbl lk;

  dim = 2*tree->n_otu-3;
  eps = 0.001;

  hessian     = (phydbl *)mCalloc((int)dim*dim,sizeof(phydbl));

  ori_bl      = (phydbl *)mCalloc((int)dim,sizeof(phydbl));

  plus_plus   = (phydbl *)mCalloc((int)dim*dim,sizeof(phydbl));
  minus_minus = (phydbl *)mCalloc((int)dim*dim,sizeof(phydbl));
  plus_minus  = (phydbl *)mCalloc((int)dim*dim,sizeof(phydbl));
  plus_zero   = (phydbl *)mCalloc((int)dim    ,sizeof(phydbl));
  minus_zero  = (phydbl *)mCalloc((int)dim    ,sizeof(phydbl));

  tree->both_sides = 1;
  Lk(tree);

  For(i,dim) ori_bl[i] = tree->t_edges[i]->l;
  
  /* zero zero */  
  zero_zero = tree->c_lnL;

  /* plus zero */  
  For(i,dim) 
    {
      tree->t_edges[i]->l += eps * tree->t_edges[i]->l;
      lk = Lk_At_Given_Edge(tree->t_edges[i],tree);
      plus_zero[i] = lk;
      tree->t_edges[i]->l = ori_bl[i];
    }


  /* minus zero */  
  For(i,dim) 
    {
      tree->t_edges[i]->l -= eps * tree->t_edges[i]->l;
      tree->t_edges[i]->l = fabs(tree->t_edges[i]->l);
      lk = Lk_At_Given_Edge(tree->t_edges[i],tree);
      minus_zero[i] = lk;
      tree->t_edges[i]->l = ori_bl[i];
    }

  For(i,dim) Update_PMat_At_Given_Edge(tree->t_edges[i],tree);

  /* plus plus  */  
  For(i,dim)
    {
      tree->t_edges[i]->l += eps * tree->t_edges[i]->l;
      Update_PMat_At_Given_Edge(tree->t_edges[i],tree);

      For(j,3)
	if((!tree->t_edges[i]->left->tax) && (tree->t_edges[i]->left->v[j] != tree->t_edges[i]->rght))
	  Recurr_Hessian(tree->t_edges[i]->left,tree->t_edges[i]->left->v[j],1,eps,plus_plus+i*dim,tree);

      For(j,3)
	if((!tree->t_edges[i]->rght->tax) && (tree->t_edges[i]->rght->v[j] != tree->t_edges[i]->left))
	  Recurr_Hessian(tree->t_edges[i]->rght,tree->t_edges[i]->rght->v[j],1,eps,plus_plus+i*dim,tree);

      tree->t_edges[i]->l = ori_bl[i];
      Lk(tree);
    }


  /* plus minus */  
  For(i,dim)
    {
      tree->t_edges[i]->l += eps * tree->t_edges[i]->l;
      Update_PMat_At_Given_Edge(tree->t_edges[i],tree);

      For(j,3)
	if((!tree->t_edges[i]->left->tax) && (tree->t_edges[i]->left->v[j] != tree->t_edges[i]->rght))
	  Recurr_Hessian(tree->t_edges[i]->left,tree->t_edges[i]->left->v[j],-1,eps,plus_minus+i*dim,tree);

      For(j,3)
	if((!tree->t_edges[i]->rght->tax) && (tree->t_edges[i]->rght->v[j] != tree->t_edges[i]->left))
	  Recurr_Hessian(tree->t_edges[i]->rght,tree->t_edges[i]->rght->v[j],-1,eps,plus_minus+i*dim,tree);

      tree->t_edges[i]->l = ori_bl[i];
      Lk(tree);
    }



  /* minus minus */  
  For(i,dim)
    {
      tree->t_edges[i]->l -= eps * tree->t_edges[i]->l;
      tree->t_edges[i]->l = fabs(tree->t_edges[i]->l);

      Update_PMat_At_Given_Edge(tree->t_edges[i],tree);

      For(j,3)
	if((!tree->t_edges[i]->left->tax) && (tree->t_edges[i]->left->v[j] != tree->t_edges[i]->rght))
	  Recurr_Hessian(tree->t_edges[i]->left,tree->t_edges[i]->left->v[j],-1,eps,minus_minus+i*dim,tree);

      For(j,3)
	if((!tree->t_edges[i]->rght->tax) && (tree->t_edges[i]->rght->v[j] != tree->t_edges[i]->left))
	  Recurr_Hessian(tree->t_edges[i]->rght,tree->t_edges[i]->rght->v[j],-1,eps,minus_minus+i*dim,tree);
      
      tree->t_edges[i]->l = ori_bl[i];
      Lk(tree);
    }

  
  For(i,dim)
    {
      hessian[i*dim+i] = (plus_zero[i]-2*zero_zero+minus_zero[i])/(pow(eps*tree->t_edges[i]->l,2));

      for(j=i+1;j<dim;j++)
	{
	  hessian[i*dim+j] = 
	    (plus_plus[i*dim+j]-plus_minus[i*dim+j]-plus_minus[j*dim+i]+minus_minus[i*dim+j])/
	    (4*eps*tree->t_edges[i]->l*eps*tree->t_edges[j]->l);
	  hessian[j*dim+i] = hessian[i*dim+j];
	}
    }
  
  Matinv(hessian, dim, dim, plus_plus);
      
  Free(ori_bl);
  Free(plus_plus);
  Free(minus_minus);
  Free(plus_zero);
  Free(minus_zero);
  Free(plus_minus);
    
  return hessian;

}

/*********************************************************/

void Recurr_Hessian(node *a, node *d, int plus_minus, phydbl eps, phydbl *res, arbre *tree)
{
  int i;
  phydbl ori_l;

  For(i,3)
    if(a->v[i] == d)
      {
	Update_P_Lk(tree,a->b[i],a);

	ori_l = a->b[i]->l;
	if(plus_minus > 0) a->b[i]->l += eps * a->b[i]->l;
	else               a->b[i]->l -= eps * a->b[i]->l;
	a->b[i]->l = fabs(a->b[i]->l);
	res[a->b[i]->num] = Lk_At_Given_Edge(a->b[i],tree);
/* 	res[a->b[i]->num] = Return_Lk(tree); */
/* 	printf("\n>> %f %f",res[a->b[i]->num],Return_Lk(tree)); */
	a->b[i]->l = ori_l;
	Update_PMat_At_Given_Edge(a->b[i],tree);
	break;
      }

  if(d->tax) return;
  else 
    For(i,3) 
      if(d->v[i] != a) 
	Recurr_Hessian(d,d->v[i],plus_minus,eps,res,tree);
}

/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/
/*********************************************************/