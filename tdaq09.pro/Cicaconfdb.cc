#include "Cicaconfdb.h"
#include <string.h>

icaconfdb::icaconfdb(int myId):icadb("icadb","icab159.pd.infn.it")
{
  mId=myId;
  printf("adding to run %d cpuconf %d \n",curr_run,mId);
}

int
icaconfdb::insert()
{
  char mque[1024];
  
  sprintf(mque,"insert into cpuconf values (\"%d\",\"NULL\",\"%d\")",
      curr_run,mId);
   printf("Doing query %s",mque);
  int rows=0,fields=0;
  sqlquery(&mque[0],&rows,&fields);
  confid = mysql_insert_id(&mysql);
  return confid;
}

int 
icaconfdb::fep_insert(dedpar *mded)
{
  char mque[1024];
  
/*  Has to be adapted to Warp

  sprintf(mque,"insert into feparameters values (\"%d\",\"%d\", \"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\" )",
      confid,mded->mask1,mded->mask2,mded->chans,mded->rsum,mded->fsum,
       mded->rze,mded->fze,mded->wcu,mded->rne,mded->fne,
       mded->medsiz,mded->digsiz,mded->pol);
*/
   printf("Doing query %s",mque);
  int rows=0,fields=0;
  sqlquery(&mque[0],&rows,&fields);
  return rows;
}

int 
icaconfdb::trgp_insert(trigpar *mtri)
{
  char mque[1024];
  
  sprintf(mque,"insert into trigconf  values (\"%d\",\"%d\", \"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\",\"%d\" )", curr_run,mId,mtri->mode,mtri->trigmode,mtri->triga1, mtri->triga2,mtri->npre,mtri->npost,mtri->lat);
   printf("Doing query %s",mque);
  int rows=0,fields=0;
  sqlquery(&mque[0],&rows,&fields);
  return rows;
}

