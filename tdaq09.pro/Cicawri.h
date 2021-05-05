#ifndef _Cicawri_
#define _Cicawri_

#include "Cicathr.h" 
#include "Cicacrate.h" 
#include "Cicaevmhdl.h" 
#include "Cicadskfil.h" 
#include "Cicabuistat.h" 
#include "Cicaactimon.h" 
#include "Cicalogdb.h" 

#define kMaxNoCrates 50


class icawri: public icathr 
{

  icacrate *mCrateList[100];;
  int totCrates;

  icaevmhdl *mEvm;
  icadskfil *mfil;
  icalogdb mlog;
  //icadbrunfiles *dbfiles;  

  actimon *mActi;
  buistat mStat;
  bool notDone; 
 
  int evId;

  public:

   icawri(icaevmhdl *mevm,actimon *mac); 
   ~icawri();

   void DoTask(); 
   void WriteHeader();
   void Configure();
};
   
#endif
