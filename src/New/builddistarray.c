
#include <stdio.h>
#include <string.h>

double ddist(int pointsused,int i, int j, double data1[], double data2[]) {
  
}

int main(argc,argv)
int argc;
char *argv[];
{
  int pointsused=10000;
  int size1, size2;
  char fprefix1[100];
  char fprefix2[100];
  char tag1[10];
  char tag2[10];
  double data1[]=malloc(sizeof(double)*pointsused*size1));
  double data2[]=malloc(sizeof(double)*pointsused*size2));
  double dists[]=malloc(sizeof(double)*size1*size2));

  int i;
  for (i=0;i<size1;i++) {
    dists[i][i]=0;
  }
  for (i=1;i<size1;i++) {
    for (j=0;j<i;j++) {
      dists[i*sizesum+j]=ddist(pointsused,i,j,data);
      dists[j*sizesum+i]=dists[i*sizesum+j];
    }
  }
  
  
  free(data);
  free(dists);
}
