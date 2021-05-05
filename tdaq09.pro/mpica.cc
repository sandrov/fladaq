#include "Cicaevm.h"
#include <stdlib.h>

int
main (int argc, char** argv)
{
  if ((argc < 9) || (argc > 9)){
      printf("Usage: %s run totevs evs/file samples pretrig fullscale offset thresh\n",argv[0]);
    exit(-1);
  }
  if (argc > 1) 
    if (strcmp("-?",argv[1])==0)
      printf("Usage: %s run totevs evs/file samples pretrig fullscale offset thresh\n",argv[0]);

   int mrun;
   sscanf(argv[1],"%d",&mrun);
   int nEv;
   sscanf(argv[2],"%d",&nEv);
   int evsfile;
   sscanf(argv[3],"%d",&evsfile);
   int totsamples;
   sscanf(argv[4],"%d",&totsamples);
   int pretrig;
   sscanf(argv[5],"%d",&pretrig);
   int fullscale;
   sscanf(argv[6],"%d",&fullscale);
   int offset;
   sscanf(argv[7],"%d",&offset);
   int thresh;
   sscanf(argv[8],"%d",&thresh);

   icaevm mEvm;

   mEvm.theRun=mrun;
   //mEvm.maxevs=1000000;
   mEvm.maxevs=nEv;
   printf("fu %d ofs %d thre %d\n",fullscale,offset,thresh);
   mEvm.setCommonParameters(evsfile,totsamples,pretrig,fullscale,offset,thresh);
   mEvm.start();
   sleep(5);
   mEvm.cmdloop();
}
