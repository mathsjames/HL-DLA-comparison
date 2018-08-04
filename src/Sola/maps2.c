/* Routines for doing HL model */
/* by F Johansson & A Sola 2008--2014*/
/* based on C program for DLA simulation by C T McMullen*/

/* Mod-derivative of function is dfunction */
/* Usual derivative is derfunction */


//#include <math.h>
//#include <complex.h>

//typedef double _Complex complex;

static complex cx_1=1.0;
static complex cx_1pi=1.0+1.0*I;
static complex cx_1mi=1.0-1.0*I;
static complex cx_0=0.0;
static complex cx_2=2.0;
static complex cx_4=4.0;
static complex eta=1;
static complex cx_half=0.5;

/* Functions */
/*complex capac1(), capac2(),m(), derm(), mi(), dermi(), slit(), derslit(), bend(), sf(), dersf(), f();
  double dm(), dmi(), dslit(), dbend(), dsf(), df(), capf(), cp(), cv();*/


/*LogCap*/
complex capac1(a)
	double a;
{
complex z;
z=1+a*a/(4*(a+1));
return(cx_1);
} 

complex capac2(a)
	double a;
{
complex z;
z=1-a*a/(a+2)/(a+2);
return(z);
} 


/* Mobius map from Delta to R = right half plane */
/* m(z) = (z-1)/(z+1) */
complex m(z)
	complex z;
{
  return((z-1.0)/(z+1.0));
}

complex derm(z)
     complex z;
{
  return(2/(z+1.0)*(z+1.0));
}

double dm(z)
	complex z;
{
	double d;
	d=(creal(z)+1)*(creal(z)+1) + cimag(z)*cimag(z);
	return(2/d);
}

/* Mobius map from R to Delta */
/* mi(z) = (1+z)/(1-z) */
complex mi(z)
	complex z;
{
  return((1.0+z)/(1.0-z));
}

complex dermi(z)
     complex z;
{
  return(2/((1-z)*(1-z)));
}


double dmi(z)
	complex z;
{
	double d;
	d=(creal(z)-1)*(creal(z)-1) + cimag(z)*cimag(z);
	return(2/d);
}

/* Basic slit mapping, Sqrt[z^2+1], R->R */
/* (keeps track of argument)*/
complex slit(z)
	complex z;
{
	complex w;
	w = 1+z*z;
        w=csqrt(w);
	return(w);
}


complex derslit(z)
     complex z;
{
  complex w; 
  w=z/slit(z);
  return(w);
}
  
double dslit(z)
	complex z;
{
	return(cabs(z/slit(z)));
}

/* Basic bend mapping, z + Sqrt[z^2+1], R->R */ 
/* not used in HL*/
complex bend(z)
	complex z;
{	
	return(z+slit(z));
}

double dbend(z)
	complex z;
{	
	return(cabs(1+z/slit(z)));
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
	w = m(z/t);
	w *= r;
	w = slit(w);
	w *= s;
	w = t*mi(w);
	return(w);
}

/* Derivative of slit mapping*/
complex dersf(z,t,a)
     complex z,t;
     double a;
{  
  complex w, psi, phi, fprim, r, s, q, u;
  double  b;

  s = a/(a+2);
  r = sqrt((1/creal(s))*(1/creal(s))-1);
  psi=derm(z/t);
  q=r*m(z/t);
  fprim=r*derslit(q);
  phi=s*dermi(s*slit(q));
  w=psi*fprim*phi;
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
  w = m(z/t);
  d = dm(z/t);
  w *= r;
  d *= r*dslit(w);
  w = slit(w);
  w *= s;
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
  factor2=cexp(eta*clog(sf(z,t,a)/z));
  w=factor1*factor2;
 
  return(w);
}



/* Derivative of the building block function*/
double df(z,t,a)
     complex z, t;
     double a;
{
  complex term1, term2, term3, fact1, fact2, rat1, rat2, tz, tz2;
    double w;
    fact1=f(z,t,a)/z;
    term1=(cx_1-eta)*fact1;
    fact2=eta*dersf(z,t,a);
    term2=fact2*cexp((eta-cx_1)*clog(sf(z,t,a)/z));
    w=cabs(term1+term2); 
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
	z = 1+a-1.0*I;
	z = z*z;
	z = z*z;
	z = -z;
	return(carg(z));
}

/* Argument of critical value */
double cv(a)
	double a;
{
  complex z, w, y;
  double r, s, v;

      	s = a/(a+2);
	r = sqrt((1/s)*(1/s)-1);
        z = (1/r)*I; /*=s;*/
        /*w=slit(z);
        w.x*=s; w.y*=s; 
        w=mi(w);*/
        w=mi(z);
        y=f(w,cx_1,a);
	return(carg(y));
	/*return(arg(z));*/
}
