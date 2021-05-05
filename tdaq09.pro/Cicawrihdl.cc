#include "Cicawrihdl.h"

#define kWritPort 6789
#define kListPort 6789

icawrihdl::icawrihdl(int id,int run,int nocrates):Ctcp(0)
{
 printf("###icawrihdl contructor\n"); 
 mStatus=0;
 mwst = new wst(this);
 printf("###icawrihdl contructor end\n"); 
}

icawrihdl::~icawrihdl()
{
 if (mwst) delete mwst;
}

void
icawrihdl::initialize()
{
 try {
  Connect(htonl(mId),kWritPort);
 }
 catch (...)
 {
  printf("Errors in connectng to writer port\n");
  mStatus = -1;
 }
}

void
icawrihdl::spawnReceive(trip *mtr)
{
#ifdef DEBUG
   printf("spawnReceive\n");
#endif
   mtr->evid=ntohl(mtr->evid);
   mtr->when=ntohl(mtr->when);
   mtr->type=ntohl(mtr->type);

  try{
#ifdef DEBUG
   printf("Sending trigger info #%d (size %d)\n", ntohl(mtr->evid),sizeof(trip));
#endif 
   Send((char *)mtr,sizeof(trip));
   
   char mbu[256];

#ifdef DEBUG
   printf("Waiting trig ack\n");
#endif
   Receive((char *)&mbu,8);

   int *ttok=(int *) &mbu[0];
   int *tsta=(int *) &mbu[4];
#ifdef DEBUG
   printf("Received trig ack (%d)\n",*tsta);
#endif
   if (*ttok=='tuiq') throw(0);
   if (*ttok=='orre') throw(0);
   mwst->start(); 
  }
  catch (...)
  {
   Disconnect();
   mStatus = -1;
  }

}

void 
icawrihdl::waitWriting()
{
 try
 {
  do{
   char mbu[256];
   //Receive((char *)&mbu,8);
   long *ttok=(long *) &mbu[0];
   long *tsta=(long *) &mbu[4];
   if (*ttok=='tuiq') throw(0);
   *tsta=ntohl(*tsta);
   mStatus = *tsta;
  } while (mStatus!=0);
 }
 catch (...)
 {
  printf("Error in waitWriting\n");
  Disconnect();
  mStatus = -1;
 }
}
