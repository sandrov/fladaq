#ifndef _icarunf_
#define _icarunf_
#include "Cicadb.h"

class icarunf: public icadb
{

   int runno;
   int evs;
   char fname[255];
   int mId;

  public:
   
   icarunf(int,int, int,char *);
   int insert();
};

#endif
