#ifndef _dskfil_
#define _dskfil_
#include <stdlib.h>
#include <stdio.h>


class icadskfil
{
  
   FILE *myf;
   int mFd;
 
  public : 
   int runnumber;
   int takeno;
   int mpresdisk;
   int maxdisks;
   int maxSize;
   int fileopen;

    icadskfil(int runNo, int maxFileSize,int localdisks); 
    ~icadskfil();
  
    int checkSpace();

    int moveToDone();
    int moveToDone(int);

    int Write(char *, int);

    FILE * opennewfile();

};

#endif
