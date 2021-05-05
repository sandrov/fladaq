#ifndef _Cresbrow_
#define _Cresbrow_

#include "Cicapertsk.h"
#include "Ctcpux.h"
#include "Cudpux.h"
#include "Cresoitem.h"
#include "Cicalogdb.h"

class mb;
class mbp;

class resbrow:public icathr,Ctcp
{
  mb *mpr;
  mbp *mbr;
  resolist mres; 
  icalogdb mlog;

  long lastID;
  int timeofstart;
  public:

  resbrow(int port);
  ~resbrow();

  void browse();
  void DoTask();
  void httpHeader(Ctcp *);
  void htmlHeader(Ctcp *);
  void bodyHeader(Ctcp *);
  void bodyFooter(Ctcp *);
  void evmStatus(Ctcp *);
  void cpuStatus(Ctcp *);
  void cpu2Status(Ctcp *);
  void wriStatus(Ctcp *);
  void logStatus(Ctcp *);
};
#endif
  
