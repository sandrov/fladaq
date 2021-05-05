#include <stdio.h>
#include "Cicarunf.h"


int main(int argc,char **argv)
{
   
   int myid; 
   sscanf(argv[1],"%d",&myid);
   printf("Writer %d\n",myid);
   int myrun; 
   sscanf(argv[3],"%d",&myrun);
   printf("run    %d\n",myrun);

  icarunf myrunf(myid,myrun,-1,argv[2]);

   myrunf.insert();
} 
