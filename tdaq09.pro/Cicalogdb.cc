#include "Cicalogdb.h"
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <time.h>

icalogdb::icalogdb():icadb("icadb","icab159.pd.infn.it")
{
  icalogdb(mesg,"");
}

icalogdb::icalogdb(msglevel mle,char *themsg):icadb("icadb","icab159.pd.infn.it")
{
  mLevel=ntohl(mle);
  strcpy(mbody,themsg);
  //printf("adding to run %d msg %s\n",curr_run,mbody);
}

int icalogdb::Tell(char *themsg)
{
  mLevel=ntohl(mesg);
  strcpy(mbody,themsg);
  return insert();
} 

int icalogdb::Warn(char *themsg)
{
  mLevel=ntohl(warn);
  strcpy(mbody,themsg);
  return insert();
} 

int icalogdb::Erro(char *themsg)
{
  mLevel=ntohl(erro);
  strcpy(mbody,themsg);
  return insert();
} 

int icalogdb::insert()
{
  char mque[1024];
  char mle[5];
   
  sprintf(mque,"insert into logger values (NULL,NULL,\"%4.4s\",\"%s\",\"%d\")",
      &mLevel,mbody,curr_run);
   printf("Doing query %s",mque);
  int rows=0,fields=0;
  return sqlquery(&mque[0],&rows,&fields);
}

int icalogdb::fetchlast(char *msgstr)
{
  int rows=0,fields=0;
  sqlquery("select max(msgid) from logger;",&rows,&fields);
  if (rows>0)
  {
    fetchrow();
    char myfie[1024];
    char mquer[1024];
    returnfield(0,&myfie[0]);
   
    //printf("fetchast retrieved %s\n",myfie);
    int lstid;
    sscanf(myfie,"%d",&lstid);
  
    sprintf(mquer,"select datetime,runid,body from logger \
      where msgid=%d",lstid);
   //printf("doing query\n%s \n",mquer);
    sqlquery(&mquer[0],&rows,&fields);
   printf("done query\n%s \n",mquer);
    printf("retrieved %d rows and %d fields\n",rows,fields);
    fetchrow();
    *msgstr=0;
    returnfield(0,&myfie[0]);
    struct tm myt;
    strptime (myfie,"%Y%m%d%H%M%S",&myt);
    time_t tim=mktime(&myt);
    strcpy(msgstr,ctime(&tim));
    msgstr[strlen(msgstr)-1]=0;
    strcat(msgstr,"  Run#");
    for(int ii=1;ii<fields;ii++)
    {
      int mlen=returnfield(ii,&myfie[0]);
      strncat(msgstr,myfie,mlen); 
      strcat(msgstr,"   "); 
    }
    return 0;
   }
    return -1;
}
