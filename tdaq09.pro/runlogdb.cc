#include <stdio.h>
#include "Cicalogdb.h"


int main(int argc,char **argv)
{
   
   int myid; 
   sscanf(argv[1],"%d",&myid);
   printf("Writer %d\n",myid);
   int myrun; 
   sscanf(argv[3],"%d",&myrun);
   printf("run    %d\n",myrun);

  icalogdb myrunf(mesg,argv[2]);

   char lstmsg[1024];
   myrunf.fetchlast(lstmsg);
   printf("Last message :%s\n",lstmsg);

}  
