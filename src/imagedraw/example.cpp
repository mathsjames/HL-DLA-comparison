#include<complex.h>
#include<stdio.h>
#include<string.h>

#include "bitmap/bitmap_image.hpp"

int main(int argc, char** argv)
{
  char readfile[100];
  char writefile[100];
  int particleNumber, imageSize, colorbool;
  float eps;
  colorbool=0;
  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {
    case 'r':
      //      printf("r\n");
      sscanf(argv[++i],"%s",readfile);
      break;

    case 'w':
      //printf("w\n");
      sscanf(argv[++i],"%s",writefile);
      break;

    case 'p':
      //printf("p\n");
      sscanf(argv[++i],"%d",&particleNumber);
      break;

    case 'e':
      //printf("e\n");
      sscanf(argv[++i],"%f",&eps);
      break;

    case 's':
      //printf("s\n");
      sscanf(argv[++i],"%d",&imageSize);
      break;

    case 'c':
      //printf("c\n");
      sscanf(argv[++i],"%d",&colorbool);
      break;

    default:
      printf("Valid flags are:\n");
      printf("-r inputfile\n-w outputfile\n-p number of particles\n-e radius of partices\n-s image width/height in pixels\n");
      return 1;
    }
  }
  printf("loading %s\n",readfile);
  FILE *readFILE;
  double _Complex particles[particleNumber];
  readFILE = fopen(readfile,"r");
  fread(particles,sizeof(double _Complex),particleNumber+1,readFILE);
  fclose(readFILE);
  printf("loaded %s\n",readfile);
  double radius=0;
  double d;
  for(int i=0; i<particleNumber+1; i++) {
    d=creal(particles[i]);
    if (d>radius) {radius=d;};
    d=cimag(particles[i]);
    if (d>radius) {radius=d;};
  }
  double scaleFactor=imageSize/(2*radius);
  printf("Scale Factor is %lf\n",scaleFactor);
  eps*=scaleFactor;

  if (scaleFactor>imageSize) {
    printf("Scale factor too large, is the cluster just a point?\n");
    return 1;
  }

  
  bitmap_image image(imageSize,imageSize);
  image.set_all_channels(255, 255, 255);
  cartesian_canvas canvas(image.width(),image.height());
  canvas.image()=image;

  for (int count=0; count<particleNumber+1; count++) {
    if (colorbool) {
      canvas.pen_color(jet_colormap[(count*999)/particleNumber]);
    }
    canvas.fill_circle(creal(particles[count])*scaleFactor,cimag(particles[count])*scaleFactor,eps);
  }
  
  canvas.image().save_image(writefile);
  
  return 0;
}
