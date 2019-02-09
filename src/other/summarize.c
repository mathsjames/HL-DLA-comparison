#include <stdio.h>
#include <math.h>

#define models 10

double pvalue(double energies[100000])
{
  int tot=0;
  for (int i=0;i<100000;i++)
    {
      tot+=(energies[i]>=energies[0]);
    }
  return tot/100000.0;
}

double mean(double energies[100000])
{
  double tot=0;
  for (int i=1;i<100000;i++)
    {
      tot+=energies[i];
    }
  return tot/99999;
}

double maxpermenergy(double* energies)
{
  double max=0;
  for (int i=1; i<100000; i++)
    {
      if (energies[i]>max)
	{
	  max=energies[i];
	}
    }
  return max;
}
int main()
{
  char* fprefixes[models];
  fprefixes[0]="P2HLN100000S";
  fprefixes[1]="P1EXACTN100000S";
  fprefixes[2]="P2MCN100000S";
  fprefixes[3]="DLAMC0N";
  fprefixes[4]="DLAMC1N";
  fprefixes[5]="FDLA";
  fprefixes[6]="NR5";
  fprefixes[7]="NR7";
  fprefixes[8]="P2HLN10000S";
  fprefixes[9]="P2HLN1000000S";
  double sizes[models]={3000,500,500,16000,5000,3000,5000,3000,3000,200};
  char filename[100];
  double energies[100000];
  FILE* fp;
  double output[4][models][models];
  for (int i=0;i<models;i++)
    {
      for (int j=i+1;j<models;j++)
	{
	  sprintf(filename,"energies/energies%s-%s",fprefixes[i],fprefixes[j]);
	  if (fp=fopen(filename,"r"))
	    {
	      fread(energies,sizeof(double),100000,fp);
	      output[0][i][j]=energies[0];
	      output[1][i][j]=pvalue(energies);
	      output[2][i][j]=sqrt(energies[0]*(sizes[i]+sizes[j])/(sizes[i]*sizes[j]));
	      output[3][i][j]=maxpermenergy(energies);
	      printf("Report for %s-%s\nenergy is %lf\np-value is %lf\ndistance is %lf\navg permutation energy is %lf\n\n",fprefixes[i],fprefixes[j],output[0][i][j],output[1][i][j],output[2][i][j],mean(energies));
	      for (int o=0;o<4;o++) {
		output[o][j][i]=output[o][i][j];
	      }
	    }
	  else
	    {
	      printf("Failed to open %s\n",filename);
	    }
	}
    }
  int indicies[models]={3,4,0,1,2,5,6,7,8,9};
  char* outputs[4];
  outputs[0]="Energies";
  outputs[1]="p-values";
  outputs[2]="distances";
  outputs[3]="max perm energy";
  for (int o=0;o<4;o++)
    {
      printf("%s\n          ",outputs[o]);
      char pref[7];
      pref[6]='\0';
      for(int j=0;j<models;j++)
	{
	  for (int c=0;c<6;c++)
	    {
	      pref[c]=fprefixes[indicies[j]][c];
	    }
	  printf("%-10s",pref);
	}
      printf("\n");
      for (int i=0;i<models;i++)
	{
	  for (int c=0;c<6;c++)
	    {
	      pref[c]=fprefixes[indicies[i]][c];
	    }
	  printf("%-10s",pref);
	  int k=indicies[i];
	  for (int j=0;j<models;j++)
	    {
	      int l=indicies[j];
	      if (i<j)
		{
		  printf("%-10lf",output[o][k][l]);
		}
	      else
		{
		  printf("----------");
		}
	    }
	  printf("\n");
	}
      printf("\n");
    }
  return 0;
}

