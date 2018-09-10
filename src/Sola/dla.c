/* HL growth Main Line */
/* James Bell 2018 */
/* based on code by Johansson Viklund & A Sola 2008--2014 */
/* which was based on C program for DLA by C T McMullen */

/* Grows a HL(alpha) cluster, composed by NGEN particles. */


#include "cx.h"
#include <math.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
//#include <stdlib.h> 
#define PI 3.14159265358979

#define ALPHA 0.00
#define SPIKERR 0.01
#define SPIKE 1.41421
#define MAXAGG 100000
#define NGEN 500

/* Name of capacity file */
static char fname[90];
static char finf[80];
static char fid[70];





/* Aggregate data */
complex theta[MAXAGG];
complex location[MAXAGG];
double a[MAXAGG];
static int nagg = 0;    
static double alpha=ALPHA, spike = SPIKE, sigma = 0.0;
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
  unsigned int seed; /* if seed1 = seed2 then srandom(seed1) = srandom(seed2), i.e. same random sequence */
    
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

	case 'S':  /* default: seed = time(NULL) */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%u",&seed);
	  state=seed;
	  break;

	case 's':  /* reads sigma from user, default 0 */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&sigma);
	  break;

	default:
	  usage();
	}
    }
  grow();  /* generates list of attachment points, length of slit to be attached and locations */
  /* when grow() stops, nagg = NGEN */
  fprintf(stderr,"%d generations complete.\n",ngen);
  FILE *file;
  



  fname[0]='\0';
  strcat(fname,"location/location");
  finf[0]='\0';
  strcat(fname,fid);
  sprintf(finf, "N%d", ngen);
  strcat(fname,finf);
  sprintf(finf, "S%d", seed);
  strcat(fname,finf);
  sprintf(finf, "SIG%1.2f", sigma);
  strcat(fname,finf);
  sprintf(finf, "A%1.2f", alpha);
  strcat(fname,finf);
  
  file=fopen(fname, "w");
  fwrite(location, sizeof(double _Complex), ngen, file);
  fclose(file);
  
  fname[0]='\0';
  strcat(fname,"sizes/sizes");
  finf[0]='\0';
  strcat(fname,fid);
  sprintf(finf, "N%d", ngen);
  strcat(fname,finf);
  sprintf(finf, "S%d", seed);
  strcat(fname,finf);
  sprintf(finf, "SIG%1.2f", sigma);
  strcat(fname,finf);
  sprintf(finf, "A%1.2f", alpha);
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
  eprintf("  -s [sigma] (0)\n");
  eprintf("  -S [random seed]\n");
  exit(0);
}

/* Grow */
void grow()
{
  int i;
  for(i=1; i<ngen; i++) {
    if (i%1000==0) {
      printf("%d percent done\n", i/1000);
    }
    aggregate();  /* writes on file capacity and slit length */
  }
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
  midpoint.x=theta[nagg].x*scale;
  midpoint.y=theta[nagg].y*scale;
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
  rr.x=1.0; rr.y=0;
  del=1.000001;
  tdel.x=del*t.x; tdel.y=del*t.y;
  /*approximate derivative*/
  /*D=cabs(div(sub(map(mult(rr,tdel)),map(mult(rr,t))), sub(mult(rr,tdel),mult(rr,t))));
    d=spike/pow(D, alpha/2);*/

  /*Makarov-type particles*/
  /*limd=2.72;
    d=limd/sqrt((nagg+1));*/

  /*exact derivative*/  
  d=spike/pow(dmap(mult(rr,t)), alpha/2);
  
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

  t=polar(1.0,2*PI*u);
  
  return(t);  
}

unsigned int myrand() {
  state=(state*1103515245+12345)%2147483648; //Same parameters as the LCG used by GCC
  return state;
}

double randu()
{
  return ((double)myrand())/((double) 2147483648);
}
