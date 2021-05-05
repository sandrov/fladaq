#include "Cicawri.h"
#include "Cicaevmhdl.h"
#include <string.h>

#define kListPort 6789

int
main (int argc, char** argv)
{

   if (argc > 1)
    if (strcmp("-?",argv[1])==0)
     printf("Usage: %s port\n",argv[0]);

  int mport=kListPort;

   if (argc > 1)
    sscanf(argv[1],"%d",&mport);

    
   Ctcp srv(6789);

   for (;;)
   {
     icawri *mWri;
     icaevmhdl  mEvm;

     printf("Waiting EVM connection...\n");
     
     try
     {
       
       actimon mActimon(0,0);

       mEvm.mcon = srv.Accept();

       mWri = new icawri(&mEvm,&mActimon);

       mWri->start();

       mWri->join();
       delete mWri;
     }
     catch (...)
     {
       if (mWri) delete mWri;
     }
   }
} 
