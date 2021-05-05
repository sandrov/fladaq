#ifndef _evmhdl_
#define _evmhdl_
#include "wrimsg.h"
#include "Ctcpux.h"

class icaevmhdl 
{
  public:

    Ctcp *mcon;

    trip mtr;
    wriparam wpa;

   icaevmhdl();
   ~icaevmhdl();
  
   void handleInit();
   int waitTrig();
   void receivingDone();
   void giveStatus(int);
   void giveUp();
  
};

#endif
