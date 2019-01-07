#include <stdio.h>

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

int main()
{
  char* fprefixes[5];
  fprefixes[0]="P2HLN100000S";
  fprefixes[1]="P1EXACTN100000S";
  fprefixes[2]="P2MCN100000S";
  fprefixes[3]="DLAMC0N";
  fprefixes[4]="DLAMC1N";
  char filename[100];
  double energies[100000];
  FILE* fp;
  for (int i=0;i<5;i++)
    {
      for (int j=i+1;j<5;j++)
	{
	  sprintf(filename,"energies/energies%s-%s",fprefixes[i],fprefixes[j]);
	  if (fp=fopen(filename,"r"))
	    {
	      fread(energies,sizeof(double),100000,fp);
	      printf("Report for %s-%s\nenergy is %lf\np-value is %lf\navg permutation energy is %lf\n\n",fprefixes[i],fprefixes[j],energies[0],pvalue(energies),mean(energies));
	    }
	  else
	    {
	      printf("Failed to open %s",filename);
	    }
	}
    }
  return 0;
}
  
