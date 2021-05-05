#include "Cicawd.h"
#include <signal.h>
#include <sys/time.h>
#include "Cudpux.h"

void alarm_handler(int sig)
{
   printf("Received ALARM signal. %d\n",getpid());
   //throw TimExpExc(-1);
   //   throw (1);
}

icawd::icawd(long delay)
{
  mTim=delay;
  sigset(SIGALRM, (void(*)) alarm_handler);
  alarm(delay);
}

icawd::~icawd()
{
  sigset(SIGALRM,SIG_DFL);
}

void 
icawd::start()
{
  printf("starting alarm %d\n",getpid());
  alarm(mTim);
}

main()
{
  icawd *mwd= new icawd(1);
  int aa;
  try {
    Cudp mp(12345);
    //mwd->start();
    //sleep(5);
    //pause();
    mwd->start();
    char bu[256];
    aa = mp.Receive((char *)&bu,10);
    printf("received %d chars\n",aa);
    delete mwd;
    //pause();
    
    }
   catch (TimExpExc *aa)
   {
     printf("%d - Exception from %d\n" , getpid(),aa->own);
   }
  catch (...)
  {
     printf("Any other exce\n");
  }
    printf("recieived %d chars\n",aa);
}
