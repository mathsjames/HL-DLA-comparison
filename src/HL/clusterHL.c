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

#define PI 3.14159265358979
#define ALPHA 2.00
#define SPIKE 1.41421
#define MAXAGG 100000
#define NGEN 500
#define ASIZECOUNT 100000

/* Name of capacity file */
static char fname[90];
static char finf[80];
static char fid[70];
static char ffinal[100];

static complex cx_1={1.0,0.0};





/* Aggregate data */
complex theta[MAXAGG];
complex location[MAXAGG];
double a[MAXAGG];
double actualsize[MAXAGG];
static int nagg = 0;
static double alpha=ALPHA, spike = SPIKE, sigma = 0.0;
static int ngen=NGEN, reg=1;
unsigned int particlechoice=1;
unsigned int state;
int saveasizes;
double asizedist[ASIZECOUNT];

/* Functions */
complex map(), f(), randt(), sf(), derslit();
double capf(), cp(), cv(), dmap(), df(), finda(), dsf(), randu();
void exit(), aggregate(), grow(), usage(), saveresults(), eprintf();
unsigned int myrand();

int main(argc,argv)
int argc;
char *argv[];
{
  int i;
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

	case 'g':  /* reads number generations from user, default 100 */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%d",&ngen);
	  break;

	case 'l':  /* Length of first slit */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&spike);
	  break;

	case 'p':  /* default: 1 */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%u",&particlechoice);
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
	  
	case 'r': /* Type of Regularization */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%d",&reg);
	  break;

	case 'x': /* indicates whether actual sizes should be saved, default 0 */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%d",&saveasizes);
	  break;  

	default:
	  usage();
	}
    }
  FILE *file;
  if (reg==3)
    {
    file=fopen("asizes/compact","r");
    if (!file || ASIZECOUNT!=fread(asizedist,sizeof(double),ASIZECOUNT,file))
      {
	eprintf("failed to open asizes/compact\n");
	return 1;
      }
    fclose(file);
    }
  grow();  /* generates list of attachment points, length of slit to be attached and locations */
  /* when grow() stops, nagg = NGEN */
  saveresults(seed);
}

void saveresults(int seed)
{
  FILE* file;
  fname[0]='\0';
  finf[0]='\0';
  strcat(fname,fid);
  sprintf(finf, "P%u", particlechoice);
  strcat(fname,finf);
  switch (reg)
    {
    case 0:
      sprintf(finf, "NONE");
      break;

    case 1:
      if (sigma>0)
	{
	  sprintf(finf, "SIG%1.3f", sigma);
	}
      else
	{
	  sprintf(finf, "HL");
	}
      break;

    case 2:
      sprintf(finf, "EXACT");
      break;

    case 3:
      sprintf(finf,"MC");
      break;
    }
  strcat(fname,finf);
  sprintf(finf, "N%d", ngen);
  strcat(fname,finf);
  sprintf(finf, "S%d", seed);
  strcat(fname,finf);


  ffinal[0]='\0';
  strcat(ffinal,"location/location");
  strcat(ffinal,fname);
  
  file=fopen(ffinal, "w");
  if (!file) {
    printf("error creating location file\n");
  }
  fwrite(location, sizeof(complex), ngen, file);
  fclose(file);
  
  if (saveasizes) {
    ffinal[0]='\0';
    strcat(ffinal,"asizes/asize");
    strcat(ffinal,fname);

    file=fopen(ffinal, "w");
    if (!file) {
      printf("error creating asizes file\n");
    }
    fwrite(actualsize, sizeof(double), ngen, file);
    fclose(file);
  }
}

void eprintf(s) char *s; {fprintf(stderr,"%s",s);}

void usage()   /* prints options that should be passed to the main, then exits */
{
  eprintf("Usage:  dla [options] (defaults below)\n");
  eprintf("  -a [alpha parameter] (2)\n");
  eprintf("  -g [number of generations] (500)\n");
  eprintf("  -l [length of first slit] (1.414)\n");
  eprintf("  -p [particle shape 1=slit 2=ball] (1)\n");
  eprintf("  -s [sigma] (0)\n");
  eprintf("  -S [random seed]\n");
  eprintf("  -r [regularization (0=none 1=sigma 2=exact 3=monte carlo)] (1)\n");
  eprintf("  -x [saveActualSizes] (0)\n");
  exit(0);
}

/* Grow */
void grow()
{
  int i;
  for(i=1; i<ngen; i++) {
    if (i%1000==0) {
      printf("%d percent done\n", (100*i)/ngen);
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
  double scale;
  if (particlechoice==1) {
    complex midpoint;
    scale=1+a[nagg]/2;
    midpoint.x=theta[nagg].x*scale;
    midpoint.y=theta[nagg].y*scale;
    location[nagg]=map(midpoint);
  } else if (particlechoice==2) {
    complex basepoint;
    basepoint=map(theta[nagg]);
    location[nagg]=basepoint;
    if (saveasizes) {
      double curra=a[nagg];
      scale=curra+sqrt(1+curra*curra);
      complex endpoint; 
      endpoint.x=theta[nagg].x*scale;
      endpoint.y=theta[nagg].y*scale;
      endpoint=map(endpoint);
      actualsize[nagg]=cabs(sub(basepoint,endpoint));
    }
  }
  nagg++;  /* nagg is a global variable */
}


/* Compute the size of the nth particle */

double finda(t)
     complex t;
{
  double d, dr, dl, dn, vr, vl, vn, D1, D2, spikelength;
  complex rr, zeropoint, point;
  rr.y=0;

  switch (reg)
    {
    case 0:
      d=spike;
      break;

    case 1:
      /*exact derivative for regularized model*/
      rr.x=1.0+sigma;
      d=spike/pow(dmap(mult(rr,t)), alpha/2);
      break;

    case 2:
      spikelength=spike; //falls through to next case
    case 3:
      if (reg==3)
	spikelength=asizedist[myrand()%ASIZECOUNT]/4;

      /*make displacement exactly spikelength*/
      dl=0;
      dr=spike;
      vl=0;
      zeropoint=map(t);
      point=map(mult(f(cx_1,cx_1,dr),t));
      vr=cabs(sub(zeropoint,point));
      while (vr<spikelength) {
	dl=dr;
	dr=2*dr;
	vl=vr;
	point=map(mult(f(cx_1,cx_1,dr),t));
	vr=cabs(sub(zeropoint,point));
      }
      dn=dr;
      vn=vr;
      while (vn-spikelength>0.000001 || vn-spikelength<-0.000001) {
	dn=(dr*(spikelength-vl)+dl*(vr-spikelength))/(vr-vl);
	if (dn==dr || dn==dl) {
	  break;
	}
	point=map(mult(f(cx_1,cx_1,dn),t));
	vn=cabs(sub(zeropoint,point));
	if (vn>spikelength) {
	  dr=dn;
	  vr=vn;
	} else {
	  dl=dn;
	  vl=vn;
	}
      }
      d=dn;
      break;
  
      return(d);
    }
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
