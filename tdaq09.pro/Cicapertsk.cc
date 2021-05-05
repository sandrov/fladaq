#include <stdio.h>
#include "Cicapertsk.h"


void
icapertsk::DoTask()
{
  while(1)
  {
    DoPerTask();
    usleep(usecperiod);
  }
}

void
icapertsk::DoPerTask()
{
   printf("%d - Every so and so (%d usec)\n",getpid(),usecperiod);
}
