#ifndef _icaconfdb_
#define _icaconfdb_
#include "Cicadb.h"
#include "msgs.h"

class icaconfdb: public icadb
{

   int confid;
   int mId;

  public:
   
   icaconfdb(int);
   int insert();
   int fep_insert(dedpar *);
   int trgp_insert(trigpar *);
};

#endif
