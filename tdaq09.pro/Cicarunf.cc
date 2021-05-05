#include "Cicarunf.h"
#include <string.h>

icarunf::icarunf(int myId,int runNo,int events,char *thename):icadb("test","icab2")
{
  runno=runNo;
  evs=events;
  mId=myId;
  strcpy(fname,thename);
  printf("adding to run %d filename %s\n",runno,fname);
}

int 
icarunf::insert()
{
  char mque[1024];
  
  sprintf(mque,"insert into runfiles values (\"%d\",\"%s\",\"%d\",\"%d\")",
      runno,fname,evs,mId);
   printf("Doing query %s",mque);
  int rows=0,fields=0;
  sqlquery(&mque[0],&rows,&fields);
  return rows;
}

