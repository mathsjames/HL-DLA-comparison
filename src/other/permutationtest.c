#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

double *data11, *data12, *data22;

int state;

unsigned int myrand() {
  state=(state*1103515245+12345)%2147483648; //Same parameters as the LCG used by GCC
  return state;
}

int fileread(char* fname, int size, double* data)
{
  FILE* file;
  data=malloc(sizeof(double)*size);
  if (!data) {
    printf("malloc failed\n");
  }
  if (file=fopen(fname,"r")) {
    fread(data,sizeof(double),size,file);
    fclose(file);
  }
  else {
    printf("open %s failed\n",fname);
    return 1;
  }
  return 0;
}

double getEntry(int i, int j, int size1, int size2)
{
  if (i<size1)
    {
      if (j<size1)
	{
	  return data11[i*size1+j];
	}
      else
	{
	  return data12[i*size2+j-size1];
	}
    }
  else
    {
      if (j<size1)
	{
	  return data12[j*size2+i-size1];
	}
      else
	{
	  return data22[(i-size1)*size2+j-size1];
	}
    }
}

double dist(int size1, int size2, int* subset1, int* subset2, int isIntra)
{
  double totalDist=0;
  int i,j, endj;
  for (i=isIntra;i<size1;i++)
    {
      endj=(isIntra)?i:size2;
      for (j=0;j<endj;j++)
	{
	  totalDist+=getEntry(subset1[i],subset2[j], size1, size2);
	}
    }
  return totalDist*2;
}

double energy(int size1, int size2, double inter, double intra1, double intra2)
{
  double n1=size1;
  double n2=size2;
  return (inter-n2*intra1/n1-n1*intra2/n2)/(n1+n2);
}

void shuffle(int* subset, int size)
{
  int i, j, temp;
  for (i=size-1;i>=0;i--)
    {
      j=myrand()%(i+1);
      temp=subset[i];
      subset[i]=subset[j];
      subset[j]=temp;
    }
}


double newenergy(int size1, int size2, double intra1, double intra2, double originalSum)
{
  double n1=size1;
  double n2=size2;
  double inter=originalSum-intra1-intra2;

  return energy(size1,size2,inter,intra1,intra2);
}

int main(argc,argv)
     int argc;
     char *argv[];
{
  int size1, size2, permcount;
  char fprefix1[100];
  char fprefix2[100];

  for(int i=1; i<argc; i++) {
    switch(argv[i][1]) {

    case '1':
      sscanf(argv[++i],"%d",&size1);
      break;

    case '2':
      sscanf(argv[++i],"%d",&size2);
      break;

    case 'c':
      sscanf(argv[++i],"%d",&permcount);
      break;

    case 'p':
      sscanf(argv[++i],"%s",fprefix1);
      break;

    case 'q':
      sscanf(argv[++i],"%s",fprefix2);
      break;

    case 's':
      sscanf(argv[++i],"%d",&state);
      break;
    }
  }

  double *energies;
  int *subset1, *subset2;
  
  char fname[210];
  sprintf(fname,"dists/dists%s-%s",fprefix1,fprefix1);
  if (fileread(fname,size1*size1,data11)) return 1;
  sprintf(fname,"dists/dists%s-%s",fprefix1,fprefix2);
  if (fileread(fname,size1*size2,data12)) return 1;
  sprintf(fname,"dists/dists%s-%s",fprefix2,fprefix2);
  if (fileread(fname,size2*size2,data22)) return 1;

  energies=malloc(sizeof(double)*permcount);
  subset1=malloc(sizeof(int)*(size1+size2));
  subset2=subset1+size1;
  if (!energies || !subset1 || !subset2)
    {
      printf("malloc failed\n");
      return 1;
    }

  int i;
  
  for (i=0;i<size1+size2;i++)
    {
      subset1[i]=i;
    }

  double inter, intra1, intra2, E, originalSum;
  inter=dist(size1,size2,subset1,subset2,0);
  intra1=dist(size1,size1,subset1,subset1,1);
  intra2=dist(size2,size2,subset2,subset2,1);
  E=energy(size1,size2,inter,intra1,intra2);
  originalSum=inter+intra1+intra2;

  for (i=0;i<permcount;i++)
    {
      shuffle(subset1,size1+size2);
      intra1=dist(size1,size1,subset1,subset1,1);
      intra2=dist(size2,size2,subset2,subset2,1);
      inter=originalSum-intra1-intra2;
      energies[i]=newenergy(size1,size2,inter,intra1,intra2);
    }


  FILE* file;
  sprintf(fname,"energies/energies%s-%s",fprefix1,fprefix1);

  if( access( fname, F_OK ) == -1 )
    {
      file=fopen(fname,"w");
      fwrite(&E,sizeof(double),1,file);
    }
  else
    {
      file=fopen(fname,"a");
    }
  fwrite(energies,sizeof(double),permcount,file);
  fclose(file);


  free(data11);
  free(data12);
  free(data22);
  free(subset1);
  free(subset2);
  return 0;
}
