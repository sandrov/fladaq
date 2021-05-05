#ifndef _Cicadb_
#define _Cicadb_

#include <stdio.h>
#include <mysql/mysql.h>

enum {kConnected=1,kQuerydone=2};

class icadb
{
  public:

   MYSQL mysql;
   MYSQL_RES *currentresult;
   MYSQL_ROW currentrow;
   unsigned int curr_rows;
   unsigned int curr_fields;

   unsigned int dbstatus;

   static int curr_run;
  

   icadb();
   icadb(char *name,char *host);
   ~icadb();
   int sqlquery(char *mquery,int *res_rows,int *res_fields);
   int fetchrow();
   int returnfield(int field,char *resu);

   void setcurrun(int);
   int insertnewrun(char *,char*);

};
#endif

