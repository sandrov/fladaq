#ifndef _wrihdl_
#define _wrihdl_

#include "wrimsg.h"
#include "Ctcpux.h"
#include "Cicathr.h"

class wst;

class icawrihdl: public Ctcp
{ 
  int mId;

  wst *mwst;
public: 
  int mStatus;

 icawrihdl(int,int,int);
 ~icawrihdl();

  void initialize();
  void spawnReceive(trip *);
  void waitWriting();

};

class wst:public icathr
{
  icawrihdl *mhdl;

  public:
  wst(icawrihdl *a):mhdl(a){};
 
  void DoTask() { mhdl->waitWriting();};

};
#endif
