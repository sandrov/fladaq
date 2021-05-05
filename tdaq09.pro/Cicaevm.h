#ifndef _Cicaevm_
#define _Cicaevm_
#endif
#include "Cicawrihdl.h"
#include "Cicacrate.h"
#include "Ckeycmd.h"
#include "Cresoitem.h"
#include "Cicatrg.h"
#include "Cicapertsk.h"
#include "Cicabuistat.h"
#include "Cicalogdb.h"



#define kMaxCrates 50
#define kMaxWriters 10

class daq;

class icaevm
{
  icatrg *mtrg;
  keycmd *mycmd; 

  int trigmode;
  int gEvs;
  //icalogdb mylog;
  daq *mdaq;

  int trigsperfile;
  int totalsamples;
  int presamples;
  int thresh;
  int offset;
  int fullscale;

  public:

  int gDone;
  int theRun;
  int maxevs;
  int totCrates;
  int totWrit;
  buistat gstat;

  icacrate *crates[kMaxCrates];
  icawrihdl *writs[kMaxWriters];
  int evmdone;


  icaevm();
  ~icaevm();

  void configure();
  void start();
  void stop();

  void cmdloop();
  void daqloop();

  icawrihdl *selectWriter();
  void CheckConnections();
  int FetchCrateList();
  int FetchWritList();
  void printstatus();
  void setCommonParameters(int,int,int,int,int,int);
};
