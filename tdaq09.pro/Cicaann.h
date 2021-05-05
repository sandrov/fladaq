#ifndef _icaann_
#define _icaann_

#include "Cudpux.h"
#include "Cicathr.h"
#include "packs.h"

enum {idle=0,waittrig,receiving,writing,waitdisk};

#include <stdlib.h>

class icaann: public Cudp,icathr
{

 public:

  udpBrd mbrd ;

  icaann(int port,int mclass);
  ~icaann();

  void DoTask();
};

#endif

