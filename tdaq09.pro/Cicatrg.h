#ifndef _trig_
#define _trig_
#include <stdio.h>

class icatrg
{
  long trgcount;
  FILE *myf;

  public:
  icatrg();
  ~icatrg();

  int waitTrig();
};
#endif
