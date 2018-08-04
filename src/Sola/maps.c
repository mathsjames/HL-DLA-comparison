/* Routines for doing HL model */
/* by F Johansson & A Sola 2008--2014*/
/* based on C program for DLA simulation by C T McMullen*/

/* Mod-derivative of function is dfunction */
/* Usual derivative is derfunction */

#include "cx.h"
#include <math.h>

static complex cx_1={1.0,0.0};
static complex cx_1pi={1.0,1.0};
static complex cx_1mi={1.0,-1.0};
static complex cx_0={0.0,0.0};
static complex cx_2={2.0,0.0};
static complex cx_4={4.0,0.0};
static complex eta={1,0.0};
static complex cx_half={0.5,0.0};

/* Functions */
/*complex capac1(), capac2(),m(), derm(), mi(), dermi(), slit(), derslit(), bend(), sf(), dersf(), f();
  double dm(), dmi(), dslit(), dbend(), dsf(), df(), capf(), cp(), cv();*/


/*LogCap*/
complex capac1(a)
	double a;
{
complex z;
z.x=1+a*a/(4*(a+1)); z.y=0;
return(cx_1);
} 

complex capac2(a)
	double a;
{
complex z;
z.x=1-a*a/(a+2)/(a+2); z.y=0;
return(z);
} 


/* Mobius map from Delta to R = right half plane */
/* m(z) = (z-1)/(z+1) */
complex m(z)
	complex z;
{
	return(div(sub(z,cx_1),add(z,cx_1)));
}

complex derm(z)
     complex z;
{
  return(div(cx_2,mult(add(z,cx_1),add(z,cx_1))));
}

double dm(z)
	complex z;
{
	double d;
	d=(z.x+1)*(z.x+1) + z.y*z.y;
	return(2/d);
}

/* Mobius map from R to Delta */
/* mi(z) = (1+z)/(1-z) */
complex mi(z)
	complex z;
{
  return(div(add(cx_1,z),sub(cx_1,z)));
}

complex dermi(z)
     complex z;
{
  return(div(cx_2,mult(sub(cx_1,z),sub(cx_1,z))));
}


double dmi(z)
	complex z;
{
	double d;
	d=(z.x-1)*(z.x-1) + z.y*z.y;
	return(2/d);
}

/* Basic slit mapping, Sqrt[z^2+1], R->R */
/* (keeps track of argument)*/
complex slit(z)
	complex z;
{
	complex w;
	w = add(cx_1,mult(z,z));
	if(z.y > 0)   w=contsqrt(w,cx_1pi);
	else	      w=contsqrt(w,cx_1mi);
	return(w);
}


complex derslit(z)
     complex z;
{
  complex w; 
  w=div(z,slit(z));
  return(w);
}
  
double dslit(z)
	complex z;
{
	return(cabs(div(z,slit(z))));
}

/* Basic bend mapping, z + Sqrt[z^2+1], R->R */ 
/* not used in HL*/
complex bend(z)
	complex z;
{	
	return(add(z,slit(z)));
}

double dbend(z)
	complex z;
{	
	return(cabs(add(cx_1,div(z,slit(z)))));
}

/* Map Delta -> Delta - slit centered at t*/
/* Grows a slit of length a at t, returns the image of z */
complex sf(z,t,a)
	complex z, t;
	double a;
{
	complex w;
	double r, s;
        
	s = a/(a+2);
	r = sqrt((1/s)*(1/s)-1);
	w = m(div(z,t));
	w.x *= r; w.y *= r;
	w = slit(w);
	w.x *= s; w.y *= s;
	w = mult(t,mi(w));
	return(w);
}

/* Derivative of slit mapping*/
complex dersf(z,t,a)
     complex z,t;
     double a;
{  
  complex w, psi, phi, fprim, r, s, q, u;
  double  b;

  s.x = a/(a+2); s.y=0;
  r.x = sqrt((1/s.x)*(1/s.x)-1); r.y=0;
  psi=derm(div(z,t));
  q=mult(r, m(div(z,t)));
  fprim=mult(r,derslit(q));
  phi=mult(s,dermi(mult(s,slit(q))));
  w=mult(mult(psi,fprim),phi);
  return(w);
}


double dsf(z,t,a)
	complex z, t;
	double a;
{
  complex w;
  double d, r, s;
  s = a/(a+2);
  r = sqrt((1/s)*(1/s)-1);
  w = m(div(z,t));
  d = dm(div(z,t));
  w.x *= r; w.y *= r;
  d *= r*dslit(w);
  w = slit(w);
  w.x *= s; w.y *= s;
  d *= s*dmi(w);
  return(d);
}

/* The building block function: 
   The parameter eta determines shape of bump, eta=1 is slit map
   Uses exp(eta log(f/z))*/

complex f(z,t,a)
     complex z, t;
     double a;
{
  complex w, factor1, factor2;
 
  factor1=z;
  factor2=cexp(mult(eta,clog(div(sf(z,t,a),z))));
  w=mult(factor1,factor2);
 
  return(w);
}



/* Derivative of the building block function*/
double df(z,t,a)
     complex z, t;
     double a;
{
  complex term1, term2, term3, fact1, fact2, rat1, rat2, tz, tz2;
    double w;
    fact1=div(f(z,t,a),z);
    term1=mult(sub(cx_1,eta),fact1);
    fact2=mult(eta,dersf(z,t,a));
    term2=mult(fact2,cexp(mult(sub(eta,cx_1),clog(div(sf(z,t,a),z)))));
    w=cabs(add(term1,term2)); 
    return(w); 
 

}


/* Capacity -- radius of mapping */
double capf(a)
	double a;
{
	double c, r, s;

	r=2*(1+a)/(a*(2+a));
        c=sqrt(1+r*r)/r;
	return(c);
}

/* Argument of critical point */
double cp(a)
	double a;
{
	complex z;
	z.x = 1+a;
	z.y = -1.0;
	z = mult(z,z);
	z = mult(z,z);
	z.x = -z.x;
	z.y = -z.y;
	return(arg(z));
}

/* Argument of critical value */
double cv(a)
	double a;
{
  complex z, w, y;
  double r, s, v;

      	s = a/(a+2);
	r = sqrt((1/s)*(1/s)-1);
        z.x=0.0, z.y=1/r; /*=s;*/
        /*w=slit(z);
        w.x*=s; w.y*=s; 
        w=mi(w);*/
        w=mi(z);
        y=f(w,cx_1,a);
	return(arg(y));
	/*return(arg(z));*/
}
