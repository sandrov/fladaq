#include "Cicadb.h"


main ()
{
  icadb mdb("test","localhost");
  char buf[131072];  

  printf("Starting..\n");
  printf("Try to set a new run");

  int myrun = mdb.insertnewrun("test","auto run number"); 

  printf(", result %d \n",myrun); 

  int fie,rows;
  mdb.sqlquery("select * from icarun ",&rows,&fie);

  printf("returned %d row of %d fields\n",rows,fie);

  for (int ii=0;ii<rows;ii++)
  {
   mdb.fetchrow();
   for (int jj=0;jj<fie;jj++)
    printf("[%.*s] ", (int) mdb.returnfield(jj,buf) ,buf);
   printf("\n");
  }
}
