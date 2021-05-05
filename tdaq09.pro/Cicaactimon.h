#ifndef _actimon_
#define _actimon_

#include "Cicaann.h"

class actimon: public icaann
{

  public:

    actimon(int port,int mclass):icaann(port,mclass){};
     

    void Status(int);
    void Status(int,int);
    void Status(int,int,int,int,int);
};

#endif
