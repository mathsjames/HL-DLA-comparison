#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <algorithm>

int main(int argc, char** argv) {
  char filename[100];
  double _Complex zero = 0.0+I*0.0;
  sscanf(argv[1],"%s",filename);
  FILE* fp;
  fp=fopen(filename,"a");
  if (fp)
    {
    fwrite(&zero,sizeof(double _Complex),1,fp);
    fclose(fp);
    }
  else
    {
      printf("File %s not found\n",filename);
      return 1;
    }
  return 0;
}
