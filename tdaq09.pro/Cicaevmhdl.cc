#include "Cicaevmhdl.h"
#include <string.h>


icaevmhdl::icaevmhdl()
{
}

icaevmhdl::~icaevmhdl()
{
}

void
icaevmhdl::handleInit()
{
  printf("Handleinit\n");
  mcon->Receive((char *)&wpa,sizeof(wriparam));

  wpa.runno = ntohl(wpa.runno);
  wpa.nocrates = ntohl(wpa.nocrates);
  wpa.evsperfile = ntohl(wpa.evsperfile);
  
  printf("Init run %d crates %d evs %d\n",wpa.runno,wpa.nocrates,wpa.evsperfile);
  for (int ii=0;ii<wpa.nocrates;ii++)
   wpa.crateid[ii]=ntohl( wpa.crateid[ii]);
}

int
icaevmhdl::waitTrig()
{
#ifdef DEBUG
  printf ("waiting trig packet (first4)\n");
#endif
  do { 
      mcon->Receive((char *)&mtr,4);   
  } while (strncmp((char *)&mtr,"uhuh",4)==0);
#ifdef DEBUG
  printf ("received trig packet (first4)\n");
#endif
      
   mcon->Receive((char *)&mtr+4,sizeof(trip)-4);

#ifdef DEBUG
  printf ("received trig packet (rest)\n");
#endif
   mtr.evid=ntohl(mtr.evid);
   mtr.when=ntohl(mtr.when);
   mtr.type=ntohl(mtr.type);
 
  char dump[64];
  strncpy(dump,(char *)&mtr,12);
#ifdef DEBUG
  printf ("received trig # %s %x %d\n", dump,mtr.evid,mtr.evid);
#endif
  return mtr.evid;
}

void
icaevmhdl::receivingDone()
{
  mcon->Send("done",4);
  mcon->Send((char *)&mtr.evid,4);
}

void 
icaevmhdl::giveStatus(int wsta)
{
  int msta = ntohl(wsta);

  mcon->Send("here",4);
  mcon->Send((char *)&msta,4);
}

void
icaevmhdl::giveUp()
{
 mcon->Send("quit0000",8);
 delete mcon;
}
