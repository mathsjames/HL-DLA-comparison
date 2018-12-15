#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <algorithm>

int comp(const void *xp,const void *yp) {
  double x = *((double *) xp);
  double y = *((double *) yp);
  if (x < y) {
    return -1;
  }
  if (x > y) {
    return 1;
  }
  return 0;
}

int state;

int myrand() {
  state=(state*1103515245+12345)%2147483648; //Same parameters as the LCG used by GCC
  return state;
}

double setDistance(double _Complex* points,int length,double _Complex point) {
  double distance=cabs(points[0]-point);
  for(int i=1; i<length; i++) {
    double d=cabs(points[i]-point);
    if (d<distance) {distance=d;};
  }
  return distance;
}

int comp (double f,double s) 
{
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}

int main(int argc, char** argv) {
  char readfile[100];
  char writefile[100];
  int trialnum;
  int particleNumber;
  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {
    case 'r':
      sscanf(argv[++i],"%s",readfile);
      break;

    case 'w':
      sscanf(argv[++i],"%s",writefile);
      break;

    case 'n':
      sscanf(argv[++i],"%d",&trialnum);
      break;

    case 'p':
      sscanf(argv[++i],"%d",&particleNumber);
      break;

    case 's':
      sscanf(argv[++i],"%d",&state);
      break;

    default:
      printf("Valid flags are:\n");
      printf("-r inputfile\n-w outputfile\n-n number of points to measure from\n-p number of particles\n-s random seed\n");
      return 1;
    }
  }
  FILE *readFILE;
  double _Complex particles[particleNumber];
  readFILE = fopen(readfile,"r");
  if (!readFILE || particleNumber+1!=fread(particles,sizeof(double _Complex),particleNumber+1,readFILE))
    {
      printf("failed to read particles data\n");
      return 1;
    }
  fclose(readFILE);

  double radius=0;
  for(int i=0; i<particleNumber; i++) {
    double d=cabs(particles[i]);
    if (d>radius) {radius=d;};
  }
  
  double distances[trialnum];
  double _Complex point;
  int percentdone = 0;
  for(int i=0; i<trialnum; i++) {
    int notfound=1;
    while (notfound) {
	point=(((double) myrand())/(double) 2147483648)+(((double) myrand())/(double) 2147483648)*I;
	point=(point*2-1-1*I);
	if (cabs(point)<1) {
	    notfound=0;
	    point=point*(radius*1.1);
	  }
      }
    distances[i]=setDistance(particles,particleNumber+1,point)/radius;
    /* if ((100*i)/trialnum>percentdone) {
      percentdone=(100*i)/trialnum;
      printf("%d percent done\n",percentdone);
    } */
  }

  qsort (distances, trialnum, sizeof(double), comp );
  
  FILE *writeFILE;
  writeFILE = fopen(writefile,"w");
  fwrite(distances,sizeof(double), trialnum, writeFILE);
  fclose(writeFILE);
}
