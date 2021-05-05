#include "stdio.h"
#include "Cicatrg.h"
main()
{
  unsigned long a;

  a =0xff00ff00;

  //fscanf(stdin,"%x",&a);
  printf( "%x %x %x\n", a, ~a ,-1-a);

  icatrg *mtrg=new icatrg;
  for(int i=0;i<10000;i++)
   {int mt=mtrg->waitTrig();
    printf ("received %d\n",mt);
//    usleep(12);
   }
  delete mtrg;
}
