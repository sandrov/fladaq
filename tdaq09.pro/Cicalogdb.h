#ifndef _icalogdb_
#define _icalogdb_
#include "Cicadb.h"

enum msglevel {mesg='mesg',warn='warn',erro='erro'};

class icalogdb: public icadb
{

   char mbody[1024];
   int mLevel;

  public:
   
   icalogdb();
   icalogdb(msglevel,char *);
   int insert();
   int fetchlast(char *);
   int Tell(char *);
   int Warn(char *);
   int Erro(char *);
};

#endif
