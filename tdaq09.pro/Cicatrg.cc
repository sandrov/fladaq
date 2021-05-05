#include "Cicatrg.h"

icatrg::icatrg()
{
   myf = fopen("/proc/sched","r");
   if (!myf) 
   {
      printf("No trigger board running on this machine! Exiting.\n");
      throw (0);
   }
   trgcount=0;
}

icatrg::~icatrg()
{
  fclose (myf);
}

int
icatrg::waitTrig()
{
  char lbu[256];

   while (fgets(lbu,256,myf)==0) ;
  return trgcount++;
}
