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

#define ALPHA 2.00
#define SPIKERR 0.01
#define SPIKE 1.41421
#define MAXAGG 100000
#define NGEN 500

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
static double alpha=ALPHA, spike = SPIKE, sigma = 0.0, sigonspike = 0.5, capacity=1.0;
static int ngen=NGEN, reg=1;
complex eta;
unsigned int particlechoice=1;
unsigned int state;
int saveasizes;
double asizedist[100000];

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
  eta.x=1.0;
  eta.y=0.0;
  for(i=1; i<argc; i++)
    {	if(argv[i][0] != '-') usage();
      switch(argv[i][1])
	{
	case 'a':
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&alpha);  /* %lf = double */
	  break;

	case 'e':
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&(eta.x));  /* %lf = double */
	  break;

	case 'f':  /* reads name of capacity file */
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%s",fid);  /* %s = string */
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

	case 'z': /*sigonspike*/
	  if(argc <= (i+1)) usage();
	  sscanf(argv[++i],"%lf",&sigonspike);
	  break;

	default:
	  usage();
	}
    }
  FILE *file;
  if (reg==7) {
    file=fopen("asizes/compact","r");
    fread(asizedist,sizeof(double),1000,file);
    fclose(file);
  }
  grow();  /* generates list of attachment points, length of slit to be attached and locations */
  /* when grow() stops, nagg = NGEN */
  fprintf(stderr,"%d generations complete.\n",ngen);

  
  fname[0]='\0';
  finf[0]='\0';
  strcat(fname,fid);
  if (particlechoice>1) {
    sprintf(finf, "P%u", particlechoice);
    strcat(fname,finf);
  }
  switch (reg)
    {
    case 0:
      sprintf(finf, "NONE");
      break;

    case 1:
      sprintf(finf, "SIG%1.3f", sigma);
      break;

    case 2:
      sprintf(finf, "EXACT");
      break;
      
    case 3:
      sprintf(finf, "CAP%1.2f",sigonspike);
      break;

    case 4:
      sprintf(finf, "SPD%1.2f",sigonspike);
      break;

    case 5:
      sprintf(finf, "AVG");
      break;
      
    case 6:
      sprintf(finf,"SEC");
      break;

    case 7:
      sprintf(finf,"MC");
      break;
    }
  strcat(fname,finf);
  sprintf(finf, "L%1.3f", spike);
  strcat(fname,finf);
  sprintf(finf, "A%1.2f", alpha);
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
  /*
  ffinal[0]='\0';
  strcat(ffinal,"sizes/size");
  strcat(ffinal,fname);
  
  file=fopen(ffinal, "w");
  if (!file) {
    printf("error creating sizes file\n");
  }
  fwrite(a, sizeof(double), ngen, file);
  fclose(file);
  */
  /*
  double ders[10000];
  complex nt, nz;
  double ns;
  for (int k=0;k<10;k++) {
    nt.x=0;
    nt.y=(6.28*k)/10;
    nt=cexp(nt);
    for (int l=0;l<1000;l++) {
      nz=nt;
      nz.x *= 1+l*0.0005;
      nz.y *= 1+l*0.0005;
      ders[1000*k+l]=dmap(nz);
    }
  }
  file=fopen("dercirc","a");
  fwrite(ders, sizeof(double),10000, file);
  fclose(file);

  
  complex ders[1000];
  complex nt, nz;
  double ns;
  for (int k=0;k<10;k++) {
    nt.x=0;
    nt.y=(6.28*k)/10;
    nt=cexp(nt);
    for (int l=0;l<100;l++) {
      nz=nt;
      nz.x *= 1+l*0.02;
      nz.y *= 1+l*0.02;
      ders[100*k+l]=f(nz,cx_1,0.4);
    }
  }
  file=fopen("circf","a");
  fwrite(ders, sizeof(complex),1000, file);
  fclose(file); */
}

void eprintf(s) char *s; {fprintf(stderr,s);}

void usage()   /* prints options that should be passed to the main, then exits */
{
  eprintf("Usage:  dla [options] (defaults below)\n");
  eprintf("  -a [alpha parameter] (2)\n");
  eprintf("  -f [file identifier] ()\n");
  eprintf("  -g [number of generations] (500)\n");
  eprintf("  -l [length of first slit] (1.414)\n");
  eprintf("  -s [sigma] (0)\n");
  eprintf("  -S [random seed]\n");
  eprintf("  -r [regularization (0=none 1=sigma 2=exact 3=cap 4=sigpropd)] (1)\n");
  eprintf("  -z [sigonspike] (0.5)\n");
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
    capacity*=capf(a[nagg]);
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
/* For naively regularized model choose exp() */

/*double finda(t)*/
double finda(t)
     complex t;
{
  double d, dr, dl, dn, vr, vl, vn, D1, D2, randspike;
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
      /*make displacement exact*/
      dl=0;
      dr=1;
      vl=0;
      zeropoint=map(t);
      //printf("nagg=%d\n",nagg);
      //printf("%lf %lf\n",zeropoint.x,zeropoint.y);
      //rr.x=1+dr;
      point=map(mult(f(cx_1,cx_1,dr),t));
      vr=cabs(sub(zeropoint,point));
      while (vr<spike) {
	dl=dr;
	dr=2*dr;
	//rr.x=1+dr;
	vl=vr;
	point=map(mult(f(cx_1,cx_1,dr),t));
	//printf("%lf+%lfi, %lf+%lfi\n",mult(f(cx_1,cx_1,dr),t).x,mult(f(cx_1,cx_1,dr),t).y,point.x,point.y);
	vr=cabs(sub(zeropoint,point));
	//printf("dr=%lf, vr=%lf\n",dr,vr);
      }
      //printf("Found Upper Bound\n");
      dn=dr;
      vn=vr;
      while (vn-spike>0.000001 || vn-spike<-0.000001) {
	dn=(dr*(spike-vl)+dl*(vr-spike))/(vr-vl);
	if (dn==dr || dn==dl) {
	  break;
	}
	//rr.x=1+dn;
	point=map(mult(f(cx_1,cx_1,dn),t));
	vn=cabs(sub(zeropoint,point));
	if (vn>spike) {
	  dr=dn;
	  vr=vn;
	} else {
	  dl=dn;
	  vl=vn;
	}
	//printf("%lf %lf %lf %lf\n",dn,vn,point.x,point.y);
	//printf("%lf %lf %lf %lf\n",dl,dr,vl,vr);
      }
      d=dn;
      break;

    case 3:
      /*choose sigma from capacity*/
      sigma=(spike*sigonspike/pow(capacity,0.333));
      rr.x=1.0+sigma;
      d=spike*pow(capacity,alpha/2-1)/pow(dmap(mult(rr,t)),alpha/2);
      //printf("cap %f, sig %f, d %f\n",capacity,sigma,d);
      break;

    case 4:
      /*make sigma=d exactly */
      dl=0;
      dr=1;
      vl=0;
      rr.x=1+dr;
      vr=dr*dmap(mult(rr,t));
      while (vr<sigonspike*spike) {
	dl=dr;
	dr=2*dr;
	rr.x=1+dr;
	vl=vr;
	vr=dr*dmap(mult(rr,t));
      }
      dn=dr;
      vn=vr;
      while (vn-sigonspike*spike>0.000001 || vn-sigonspike*spike<-0.000001) {
	dn=(dr*(sigonspike*spike-vl)+dl*(vr-sigonspike*spike))/(vr-vl);
	rr.x=1+dn;
	vn=dn*dmap(mult(rr,t));
	if (vn>sigonspike*spike) {
	  dr=dn;
	  vr=vn;
	} else {
	  dl=dn;
	  vl=vn;
	}
      }
      d=dn;
      break;

    case 5:
      /* geometric average of both extremes*/
      d=sqrt(dmap(t)*capacity);
      //if(capacity>d) {
      //	d=capacity;
      //}
      d=spike/pow(d, alpha/2);
      break;

    case 6:
      /*second order taylor*/
      rr.y=1.000001;
      D1=dmap(t);
      D2=(rr.y-1)*(dmap(mult(t,rr))-D1);
      if (D2<-D1*D1/(2*spike)) {
	printf("cut on %d \n",nagg);
	d=2*spike/D1;
      } else {

	if (D2<0.0000000000001) {
	  if (D2>-0.000001/D1) {
	    d=spike/D1;
	  } else {
	    d=-D1/D2-sqrt(D1*D1/(D2*D2)+2*spike/D2);
	  }
	} else {
	  d=-D1/D2+sqrt(D1*D1/(D2*D2)+2*spike/D2);
	}
      }
      printf("%d %f %f %f\n",nagg,d,D1,D2);
      break;
    

    case 7:
      /*make displacement exact but random*/
      randspike=asizedist[myrand()%100000];
      dl=0;
      dr=1;
      vl=0;
      zeropoint=map(t);
      //printf("%lf %lf\n",zeropoint.x,zeropoint.y);
      //rr.x=1+dr;
      point=map(mult(f(cx_1,cx_1,dr),t));
      vr=cabs(sub(zeropoint,point));
      while (vr<randspike) {
	dl=dr;
	dr=2*dr;
	//rr.x=1+dr;
	vl=vr;
	point=map(mult(f(cx_1,cx_1,dr),t));
	//printf("%lf+%lfi, %lf+%lfi\n",mult(f(cx_1,cx_1,dr),t).x,mult(f(cx_1,cx_1,dr),t).y,point.x,point.y);
	vr=cabs(sub(zeropoint,point));
	//printf("dr=%lf, vr=%lf\n",dr,vr);
      }
      //printf("Found Upper Bound\n");
      dn=dr;
      vn=vr;
      while (vn-randspike>0.000001 || vn-randspike<-0.000001) {
	dn=(dr*(randspike-vl)+dl*(vr-randspike))/(vr-vl);
	if (dn==dr || dn==dl) {
	  break;
	}
	//rr.x=1+dn;
	point=map(mult(f(cx_1,cx_1,dn),t));
	vn=cabs(sub(zeropoint,point));
	if (vn>randspike) {
	  dr=dn;
	  vr=vn;
	} else {
	  dl=dn;
	  vl=vn;
	}
	//printf("%lf %lf %lf %lf\n",dn,vn,point.x,point.y);
	//printf("%lf %lf %lf %lf\n",dl,dr,vl,vr);
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
