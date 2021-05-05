#ifndef _pertsk_
#define _pertsk_

#include "Cicathr.h"


class icapertsk: public icathr
{
   int usecperiod;

   public:
 
    icapertsk(int mper):usecperiod(mper){};

    virtual void DoPerTask();
    void DoTask();
};
#endif
