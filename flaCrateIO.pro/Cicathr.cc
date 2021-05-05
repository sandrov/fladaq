#include "Cicathr.h"
#include <stdio.h>

void * thrtask(void *);

icathr::icathr()
{

  ownpid = getpid();
}

icathr::~icathr()
{
  pthread_cancel(mthr);
}

void
icathr::stop()
{
  pthread_cancel(mthr);
}

void 
icathr::start()
{
  pthread_create(&mthr,NULL,thrtask,(void *)this );
}

void 
icathr::join()
{
 void *aa;
 pthread_join(mthr,(void **)&aa);
}

void 
icathr::detach()
{
 void *aa;
 pthread_detach(mthr);
}

void 
icathr::DoTask()
{
  printf("Here I am...\n");
}

void *
thrtask(void *mpoi)
{
  pthread_setcancelstate(PTHREAD_CANCEL_ASYNCHRONOUS,0);
  icathr * thetask = (icathr *) mpoi;
  thetask->DoTask();
}


