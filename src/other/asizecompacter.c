#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

int main() {
  FILE *file;
  double *asizes;
  double compactasizes[100000];
  int k;
  char filename[100];
  char number[10];
  double *currentpoint;
  asizes=malloc(1000000*sizeof(double));
  if (!asizes) {
    printf("malloc failed\n");
    return 1;
  }
  currentpoint=asizes;
  for (k=1;k<11;k++) {
    filename[0]='\0';
    strcat(filename,"asizes/asizeP2HLN100000S");
    sprintf(number,"%d",k);
    strcat(filename,number);
    file=fopen(filename,"r");
    if (!file) {
      printf("Failed to open file for reading\n");
      return 1;
    }
    fread(currentpoint,sizeof(double),100000,file);
    fclose(file);
    currentpoint+=100000;
  }
  
  for (k=0;k<100000;k++) {
    compactasizes[k]=asizes[10*k+4];
  }
  qsort(compactasizes,100000,sizeof(double),comp);
  
  filename[0]='\0';
  strcat(filename,"asizes/compact");
  file=fopen(filename,"w");
  if (!file) {
    printf("Failed to open file for writing\n");
    return 1;
  }
  fwrite(compactasizes,sizeof(double),100000,file);
  fclose(file);
  free(asizes);
  return 0;
}
