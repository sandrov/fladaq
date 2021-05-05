#include "Cicadb.h"
#ifdef _MACOSX_
#include <strings.h>
#else
#include <string.h>
#endif

int icadb::curr_run;

icadb::icadb()
{
  icadb("icadb","icab159.pd.infn.it");
}

icadb::icadb(char *name="test",char *host="localhost")
{
 
  mysql_init(&mysql);

  mysql_options(&mysql,MYSQL_READ_DEFAULT_GROUP,"icadb");
//TO disable DB access
  dbstatus=0;
  return;
//
  printf("connecting to database\n");
  dbstatus=kConnected;
  if (!mysql_real_connect(&mysql,host,"daqcms","daqcms",name,0,NULL,0))
  {
    fprintf(stderr, "Constructor - Failed to connect to database: Error: %s\n",
          mysql_error(&mysql));
   dbstatus=0;
  }
}

icadb::~icadb()
{
    mysql_close(&mysql);
}

int
icadb::sqlquery(char *mquery,int *res_rows,int *res_fields)
{
  if (dbstatus & kConnected)
  {
    
    printf("Cicadb::sqlquery -- query string %s\n",mquery);
    if( ! mysql_query(&mysql,mquery))
    {printf("Cicadb::sqlquery -- querydone\n" );

      currentresult = mysql_store_result(&mysql);

      if (currentresult )
      { 
    
       curr_rows=*res_rows = mysql_num_rows(currentresult);

       curr_fields=*res_fields=mysql_num_fields(currentresult);

  
      }
      if ((currentresult) || (!mysql_field_count(&mysql)))
      {
        dbstatus |= kQuerydone;
        return 0;
      }
     }
  }
   dbstatus = dbstatus & kConnected;
 
    fprintf(stderr, "Failed to connect to database: Error\n");
   return -1;
}

int
icadb::fetchrow()
{
    currentrow = mysql_fetch_row(currentresult);
}

int 
icadb::returnfield(int field,char *resu)
{

   if (currentrow)
   {
    long unsigned int* lengths = mysql_fetch_lengths(currentresult);

    strncpy(resu,currentrow[field],(int) lengths[field]);
    
    resu[lengths[field]]=0;
    return lengths[field];
   }
  else return 0;
} 

void 
icadb::setcurrun(int nrun)
{
  curr_run = nrun;
}

int
icadb::insertnewrun(char *runtype,char *runcomment)
{
  if (dbstatus & kConnected)
  {
    char qstr[256];
    sprintf (qstr,"INSERT INTO icarun (run,runtimestamp,type, runcomment) VALUES (\"%d\",NULL,'%s','%s')",curr_run,runtype,runcomment) ;

   // printf("Cicadb -- query string %s\n",qstr);
    int da,db;
    sqlquery(qstr,&da,&db);
   
    //curr_run = mysql_insert_id(&mysql);
  
    if (mysql_error(&mysql)[0] == 0 )
        return curr_run;
  }
  
  return -1;
}

