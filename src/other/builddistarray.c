#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

double ddist(int pointsused,int i, int j, double data1[], double data2[], double lowerLimit) {
  double *a;
  a=data1+i*pointsused;
  double *b;
  b=data2+j*pointsused;
  int p1=0, p2=0;
  double tot=0, c=lowerLimit, l;
  while (p1<pointsused && p2<pointsused) {
    if (a[p1]<b[p2]) {
      if (a[p1]>c)
	{
	  l=a[p1]-c;
	  tot+=l*(p1-p2)*(p1-p2);
	  c=a[p1];
	}
      p1++;
    } else {
      if (b[p2]>c)
	{
	  l=b[p2]-c;
	  tot+=l*(p1-p2)*(p1-p2);
	  c=b[p2];
	}
      p2++;
    }
  }
  while (p1<pointsused) {
    if (a[p1]>c)
      {
	l=a[p1]-c;
	tot+=l*(p1-p2)*(p1-p2);
	c=a[p1];
      }
    p1++;
  }
  while (p2<pointsused) {
    if (b[p2]>c)
      {
	l=b[p2]-c;
	tot+=l*(p1-p2)*(p1-p2);
	c=b[p2];
      }
    p2++;
  }
  
  return sqrt(tot/(pointsused*pointsused));
}

int main(argc,argv)
     int argc;
     char *argv[];
{
  int pointsused=10000;
  int size1, size2;
  char fprefix1[100];
  char fprefix2[100];
  int reflexive;
  double lowerLimit;

  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {

    case '1':
      sscanf(argv[++i],"%d",&size1);
      break;

    case '2':
      sscanf(argv[++i],"%d",&size2);
      break;

    case 'c':
      sscanf(argv[++i],"%d",&pointsused);
      break;

    case 'l':
      sscanf(argv[++i],"%lf",&lowerLimit);

    case 'p':
      sscanf(argv[++i],"%s",fprefix1);
      break;

    case 'q':
      sscanf(argv[++i],"%s",fprefix2);
      break;

    case 'r':
      sscanf(argv[++i],"%d",&reflexive);
      break;
    }
  }
      
  double *data1;
  data1=malloc(sizeof(double)*pointsused*size1);
  double *data2;
  data2=malloc(sizeof(double)*pointsused*size2);
  double *dists;
  dists=malloc(sizeof(double)*size1*size2);
  if (!dists || !data2 || !data1)
    {
      printf("malloc failed\n");
      return 1;
    }
  int i, j, loadcount;
  char fname[210];
  FILE* file;

  for ( loadcount=1;loadcount<=size1;loadcount++) {
    sprintf(fname,"%s%d",fprefix1,loadcount);
    if (file=fopen(fname,"r")) {
      fread(data1+pointsused*(loadcount-1),sizeof(double),pointsused,file);
      fclose(file);
    } else {
      printf("failed to open %s\n",fname);
      return 1;
    }
  }


  if (reflexive) {
    for (i=0;i<size1;i++) {
      dists[i*size1+i]=0;
    }
    for (i=1;i<size1;i++) {
      for (j=0;j<i;j++) {
	dists[i*size1+j]=ddist(pointsused,i,j,data1,data1,lowerLimit);
	dists[j*size1+i]=dists[i*size1+j];
      }
    }
  } else {
    for ( loadcount=1;loadcount<=size2;loadcount++) {
      sprintf(fname,"%s%d",fprefix2,loadcount);
      if (file=fopen(fname,"r")) {
	fread(data2+pointsused*(loadcount-1),sizeof(double),pointsused,file);
	fclose(file);
      } else {
	printf("failed to open %s\n",fname);
	return 1;
      }
    }
    for (i=0;i<size1;i++) {
      for (j=0;j<size2;j++) {
	dists[i*size2+j]=ddist(pointsused,i,j,data1,data2,lowerLimit);
      }
    }
  }
  sprintf(fname,"dists/dists%s-%s",fprefix1,fprefix2);
  if (file=fopen(fname,"w")) {
    fwrite(dists,sizeof(double),size1*size2,file);
    fclose(file);
  }

  free(data1);
  free(data2);
  free(dists);
  return 0;
}
