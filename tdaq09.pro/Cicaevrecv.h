#ifndef _evrec_
#define _evrec_

#include "Cicathr.h"
#include "Cicacrate.h"

#define kMaxEvSize 9699328
#define kMaxPackSize 131072

class evrecv : public icathr
{
  icacrate *mcrate;
  buistat *gstat;

  public:

   long totbytes;
   char datab[kMaxEvSize];

   evrecv(icacrate *,buistat *);
   ~evrecv();

   void DoTask();
};

#endif
