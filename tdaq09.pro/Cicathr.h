#ifndef _Cicath_
#define _Cicath_

#include <unistd.h>
#include <pthread.h>

class icathr
{
  int ownpid;
  pthread_t mthr;

  public:
  
   icathr();
   ~icathr();

   void stop();
   void start();
   void join();
   void detach();
   virtual void DoTask();

};

#endif 
