#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include <math.h>
#include <string.h>

const double PI = 3.1415926535;
int state;

int myrand() {
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

void generatecluster(char* cfile,char* lfile,int particleNumber = 25000)
{
  double resetMultiplier = 1.02;
  double finishingThresholdS = 36.0;
  double particleSize = 1.0;
  double _Complex particles[particleNumber+1];
  double parents[particleNumber+1];
  particles[0] = 0.0+0.0*I;
  parents[0]=-1;
  particles[1] = randcirc();
  parents[1] = 0;
  double startDist = 2.0;

  for(int particleCount=1;particleCount<particleNumber;particleCount++)
    {
      double _Complex position=startDist*randcirc();
      while (1)
        {
	  int nearest;
	  double absVal=cabs(position);
	  if (absVal>resetMultiplier*startDist)
            {
	      double a=absVal/startDist;
	      double scaledC=(a-1)*randC();
	      position=(position/absVal)*startDist*(a+1-scaledC*I)/(scaledC-(a+1)*I);
            }
	  double distanceS = INFINITY;
	  double distance2S = INFINITY;
	  double _Complex *ptr_part=particles;
	  for (int k=0;k<particleCount+1;ptr_part++, k++)
            {
	      double _Complex nextDiff=position-*ptr_part;
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
	  if (distanceS>finishingThresholdS)
            {
	      position=position+(sqrt(distanceS)-1)*randcirc();
            }
	  else
            {
	      double d1=sqrt(distanceS);
	      double d2=sqrt(distance2S);
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
	      position+=I*y4*(particles[nearest]-position)/d1;
	      if (y2<0)
                {
		  particles[particleCount+1]=position;
		  parents[particleCount+1]=nearest;
		  startDist=max(startDist,cabs(position)+1);
		  break;
                }
            }
        }
    } 
  FILE *fp;
  char buffer[50];
  sprintf(buffer,cfile);
  fp=fopen(buffer,"w");
  fwrite(particles, sizeof(double _Complex), particleNumber+1, fp);
  fclose(fp);
  sprintf(buffer,lfile);
  fp=fopen(buffer,"w");
  fwrite(parents, sizeof(int), particleNumber+1, fp);
  fclose(fp);
}

main(int argc,char** argv)
{
  char* cfile, lfile;
  int particleNumber;
  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {
    case 's':
      sscanf(argv[++i],"%d",&state);
      break;

    case 'c':
      sscanf(argv[++i],"%s",cfile);
      break;

    case 'l':
      sscanf(argv[++i],"%s",lfile);
      break;

    case 'p':
      sscanf(argv[++i],"%d",&particleNumber);
      break;
    }
  }
  generatecluster(cfile,lfile,particleNumber);
}
