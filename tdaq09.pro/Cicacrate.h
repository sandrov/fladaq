#ifndef _crate_
#define _crate_
#include "Ctcpux.h"
#include "Cicadb.h"
#include "Cicabuistat.h"

class evrecv;

class icacrate : public Ctcp
{
   public: 
    
    char fname[256];
    char hostname[256];

   int status;
   icadb *thedb;
   evrecv *mrcv;
   icadb *mdb; 
   in_addr  mId;
   int totsam,thres,npre,fullscale,offset;

   icacrate(int addr,buistat *gstat);
   icacrate(in_addr addr,buistat *gstat);
   ~icacrate();

   int initialize();
   void tellTrig(int);
   int dataconnect();
   void setCommonParameters(int,int,int,int,int);
};
#endif
