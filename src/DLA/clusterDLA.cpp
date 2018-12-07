#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include <math.h>
#include <string.h>

const double PI = 3.1415926535;
int state;

unsigned int myrand() {
  state=(state*1103515245+12345)%2147483648; //Same parameters as the LCG used by GCC
  return state;
}

double randu()
{
  return (double)myrand()/(double) 2147483648;
}

double _Complex randcirc()
{
  return cexp(2*PI*I*randu());
}

double randC()
{
  return tan(PI*(randu()-0.5));
}

double max(double a, double b)
{
  return (a>b)?a:b;
}


struct Dists {
  double d1;
  double d2;
  int nearest;
};

  struct Dists findSquaredDistances(double _Complex position,double _Complex particles[],int particleCount) {
  double distanceS = INFINITY;
  double distance2S = INFINITY;
  int nearest;
  for (int k=0;k<particleCount+1;k++)
    {
      double _Complex nextDiff=position-particles[k];
      double nextDistS=creal(nextDiff)*creal(nextDiff)+cimag(nextDiff)*cimag(nextDiff);
      if (distance2S>nextDistS)
	{
	  if(distanceS>nextDistS)
	    {
	      distance2S=distanceS;
	      distanceS=nextDistS;
	      nearest=k;
	    }
	  else
	    {
	      distance2S=nextDistS;
	    }
	}
    }
  struct Dists retval;
  retval.d1=distanceS;
  retval.d2=distance2S;
  retval.nearest=nearest;
  return retval;
  };

struct Dists findSizeCorrectedDistances(double _Complex position,double _Complex particles[],int particleCount,double sizes[]) {
  double d1 = INFINITY;
  double d2 = INFINITY;
  double size = sizes[particleCount+1];
  double _Complex nextDiff;
  double nextDistS, nextDist;
  int nearest;
  for (int k=0;k<particleCount+1;k++)
    {
      nextDiff=position-particles[k];
      nextDistS=creal(nextDiff)*creal(nextDiff)+cimag(nextDiff)*cimag(nextDiff);
      nextDist=sqrt(nextDistS)-size-sizes[k];
      if (d2>nextDist)
	{
	  if(d1>nextDist)
	    {
	      d2=d1;
	      d1=nextDist;
	      nearest=k;
	    }
	  else
	    {
	      d2=nextDist;
	    }
	}
    }
  struct Dists retval;
  retval.d1=d1;
  retval.d2=d2;
  retval.nearest=nearest;
  return retval;
}

struct Step {
  double _Complex thestep;
  char finished;
};

struct Step findStep(double d1, double d2, double _Complex nearestOffset) {
  double theta=acos((1+(d2-1)*(d2-1)-d1*d1)/(2*(d2-1)));
  double r2=((d2-1)*(d2-1)+d1*d1-1)/(2*d1);
  double r1=sqrt(1-(d1-r2)*(d1-r2));
  double _Complex alpha=r1-r2*I;
  double _Complex beta=-r1-r2*I;
  double _Complex D=(d2-1-beta)/(d2-1-alpha);
  double _Complex y1=cpow(alpha*D/beta,PI/theta);
  double y2=creal(y1)+cimag(y1)*randC();
  double _Complex y3=cpow(y2,theta/PI);
  double _Complex y4=(-beta*y3+D*alpha)/(-y3+D);
  struct Step retval;
  retval.thestep=(I*y4*(nearestOffset)/d1);
  retval.finished=(y2<0);
  return retval;
}

void generatecluster(char cfile[], int particleNumber, int useRandomSizes)
{
  int asizesamplesize=100000;
  double resetMultiplier = 1.02;
  double finishingThresholdS = 36.0;
  double asizedist[asizesamplesize];
  double _Complex particles[particleNumber+1];
  double sizes[particleNumber+1];
  particles[0] = 0.0+0.0*I;
  sizes[0]=0.5;
  particles[1] = randcirc();
  sizes[1]=0.5;
  double maxsize=0.5;
  struct Dists dcurr;
  FILE *fp;
  
  if (useRandomSizes) {
    fp=fopen("asizes/compact","r");
    if (!fp || asizesamplesize!=fread(asizedist,sizeof(double),asizesamplesize,fp)) {
      printf("failed to load asize data\n");
    }
    fclose(fp);
    for (int k=0;k<asizesamplesize;k++) {
      maxsize=max(maxsize,asizedist[k]);
    }
  }
  double startDist = 2.0+maxsize;
  for(int particleCount=1;particleCount<particleNumber;particleCount++) {
    sizes[particleCount+1]=0.5;
    if (useRandomSizes) {
      int temp=myrand()%asizesamplesize;
      sizes[particleCount+1]=asizedist[temp];
    }
    double _Complex position=startDist*randcirc();
    while (1) {
      int nearest;
      double absVal=cabs(position);
      if (absVal>resetMultiplier*startDist) {
	double a=absVal/startDist;
	double scaledC=(a-1)*randC();
	position=(position/absVal)*startDist*(a+1-scaledC*I)/(scaledC-(a+1)*I);
      }
      if (useRandomSizes) {
	dcurr=findSizeCorrectedDistances(position,particles,particleCount,sizes);
	if (dcurr.d1*dcurr.d1>finishingThresholdS) {
	  position=position+dcurr.d1*randcirc();
	} else {
	  double scale=sizes[dcurr.nearest]+sizes[particleCount+1];
	  double d1=dcurr.d1/scale+1;
	  double d2=dcurr.d2/scale+1;
	  struct Step thisStep;
	  thisStep=findStep(d1,d2,particles[dcurr.nearest]-position);
	  position+=thisStep.thestep;
	  if (thisStep.finished) {
	    particles[particleCount+1]=position;
	    startDist=max(startDist,cabs(position)+2*maxsize);
	    break;
	  }
	}
      } else { // if all particles are the same size we use a different function to work with squared distances and avoid an excessive number of sqrt operations
	dcurr=findSquaredDistances(position,particles,particleCount);
	if (dcurr.d1>finishingThresholdS) {
	  position=position+(sqrt(dcurr.d1)-1)*randcirc();
	} else {
	  double d1=sqrt(dcurr.d1);
	  double d2=sqrt(dcurr.d2);
	  struct Step thisStep;
	  thisStep=findStep(d1,d2,particles[dcurr.nearest]-position);
	  position+=thisStep.thestep;
	  if (thisStep.finished) {
	    particles[particleCount+1]=position;
	    startDist=max(startDist,cabs(position)+1);
	    break;
	  }
	}
      }
    }
  }
  char buffer[50];
  sprintf(buffer,"%s",cfile);
  fp=fopen(buffer,"w");
  fwrite(particles, sizeof(double _Complex), particleNumber+1, fp);
  fclose(fp);
}

main(int argc,char** argv)
{
  char cfile[100];
  int useRandomSizes;
  int particleNumber;
  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {
    case 's':
      sscanf(argv[++i],"%d",&state);
      break;

    case 'c':
      sscanf(argv[++i],"%s",cfile);
      break;

    case 'r':
      sscanf(argv[++i],"%d",&useRandomSizes);
      break;

    case 'p':
      sscanf(argv[++i],"%d",&particleNumber);
      break;
    }
  }
  generatecluster(cfile,particleNumber,useRandomSizes);
}
