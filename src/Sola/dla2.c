/* HL growth Main Line */
/* James Bell 2018 */
/* based on code by Johansson Viklund & A Sola 2008--2014 */
/* which was based on C program for DLA by C T McMullen */

/* Grows a HL(alpha) cluster, composed by NGEN particles. */


#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <complex.h>
//#include <stdlib.h>
#include "maps.c"

#define PI 3.14159265358979

#define ALPHA 2.00
#define SPIKERR 0.01
#define SPIKE 1.41421
#define MAXAGG 100000
#define NGEN 500

//typedef double _Complex complex;

/* Name of capacity file */
static char fname[90];
static char finf[80];
static char fid[70];





/* Aggregate data */
complex theta[MAXAGG];
complex location[MAXAGG];
double a[MAXAGG];
static int nagg = 0;    
static double alpha=ALPHA, spike = SPIKE;
static int ngen=NGEN;
unsigned int state;

/* Functions */
complex map(), f(), randt(), sf(), derslit();
double capf(), cp(), cv(), dmap(), df(), finda(), dsf(), randu();
void exit(), aggregate(), grow(), usage();
unsigned int myrand();

int main(argc,argv)
int argc;
char *argv[];
{
  int i;
  /* int n; */
  int seed; /* if seed1 = seed2 then srandom(seed1) = srandom(seed2), i.e. same random sequence */
    
  state=time(NULL); 	/* reset random number generator */
  for(i=1; i<argc; i++)
    {	if(argv[i][0] != '-') usage();
      switch(argv[i][1])
	{
	case 'a':
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&alpha);  /* %lf = double */
	  break;

	case 'f':  /* reads name of capacity file */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%s",fid);  /* %s = string */
	  break;

	case 'g':  /* reads number generations from user, default 100 */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%d",&ngen);
	  break;

	case 's':  /* reads spike from user, default sqrt(2) */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&spike);
	  break;

	case 'S':  /* default: seed = time(NULL) */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%d",&seed);
	  state=(unsigned int) seed;
	  break;


	default:
	  usage();
	}
    }
  grow();  /* generates list of attachment points, length of slit to be attached and locations */
  /* when grow() stops, nagg = NGEN */
  fprintf(stderr,"%d generations complete.\n",ngen);
  FILE *file;
  finf[0]='\0';
  strcat(finf,fid);
  sprintf(finf, "N%d", ngen);
  sprintf(finf, "S%d", seed);
  fname[0]='\0';
  strcat(fname,"location");
  strcat(fname,finf);
  double loc2[2*ngen];
  for (int count=0;count<ngen;count++) {
    loc2[2*count-2]=creal(location[count]);
    loc2[2*count-1]=cimag(location[count]);
  }
  file=fopen(fname, "w");
  fwrite(location, sizeof(double _Complex), ngen, file);
  fclose(file);
  fname[0]='\0';
  strcat(fname,"sizes");
  strcat(fname,finf);
  file=fopen(fname, "w");
  fwrite(a, sizeof(double), ngen, file);
  fclose(file);
}

void eprintf(s) char *s; {fprintf(stderr,s);}

void usage()   /* prints options that should be passed to the main, then exits */
{
  eprintf("Usage:  dla [options] (defaults below)\n");
  eprintf("  -a [alpha parameter] (2)\n");
  eprintf("  -f [file identifier] ()\n");
  eprintf("  -g [number of generations] (500)\n");
  eprintf("  -s [size of particles] (sqrt(2))\n");
  eprintf("  -S [random seed]\n");
  exit(0);
}

/* Grow */
void grow()
{
  int i;
  for(i=1; i<ngen; i++)
    aggregate();  /* writes on file capacity and slit length */
}

/* Add to aggregate */
void aggregate()
{
  if(nagg >= MAXAGG)
    {	eprintf("Exceeded max of %d\n",MAXAGG);
      exit(0);
    }
  theta[nagg]=randt();   /* attachment point */
  a[nagg]=finda(theta[nagg]);  /* corresponding length of slit */
  complex midpoint;
  double scale=1+a[nagg]/2;
  midpoint=theta[nagg]*scale;
  location[nagg]=map(midpoint);
  nagg++;  /* nagg is a global variable */
}


/* Compute the size of the nth particle */
/* For naively regularized model choose exp() */

/*double finda(t)*/
double finda(t)
     complex t;
{
  double a, fac, dist, p, q, d, D, del, limd, C, F;
  complex rr, tdel; 
  rr=1.0;
  del=1.000001;
  tdel=del*t;

  /*Makarov-type particles*/
  /*limd=2.72;
    d=limd/sqrt((nagg+1));*/

  /*exact derivative*/  
  d=spike/pow(dmap(rr*tdel), alpha/2);
  
  return(d);
}

/* Riemann mapping to outside of aggregate */
/* Gives the image of the point z after nagg particles have grown */
complex map(z)
     complex z;       /* map(z) = F_nagg(z) */
{
  int i;
  for(i=nagg-1; i>=0; i--)
    z = f(z,theta[i],a[i]);
  return(z);
}

double dmap(z)
     complex z;
{
  int i;
  double d;
        
  d = 1;
  for(i=nagg-1; i>=0; i--) {
    d *= df(z,theta[i],a[i]);
    z = f(z,theta[i],a[i]);
  }
  return(d);
}

/* Random point on the circle */
complex randt()
{
  double u;
  complex t;
  
  u=randu();

  t=cexp(2*PI*u*I);

  return(t);  
}

unsigned int myrand() {
  state=(state*1103515245+12345)%2147483648; //Same parameters as the LCG used by GCC
  printf("%d\n",state);
  return state;
}

double randu()
{
  return (double)myrand()/(double) 2147483648;
}
