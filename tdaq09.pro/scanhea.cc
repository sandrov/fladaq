#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>

static int gDone=0;
static int mfd;
int
main(int argc, char **argv)
{
  printf("argc %d argv[1] %s\n",argc,argv[1]);
  char myfna[256];
  sscanf(argv[1],"%s",myfna);
  printf("Opening %s...\n",myfna);

  FILE * myf = fopen(myfna,"r");

  long totalbytes=0;
  long totalstat=0;
  long totalfrag=0;
  char mbu[131072];
  char *buffo=(char *)&mbu[0];
  int rdsiz;
  if (myf)
  {
    mfd = fileno(myf);
   for(;!gDone;)
   { 
      buffo=(char *)&mbu[0];
      if ((rdsiz = read(mfd,buffo,28))==28)
      {
//        printf("rdsiz %d\n",rdsiz);
        if (strncmp(buffo,"STAT",4)==0)
        {
         totalstat++;
         // printf( "statreceived %d\n" ,totalstat );
         long *evh=(long *)buffo;
         long toberead= ntohl(evh[6]);

       //   write(fde,buffo,toberead);
     //     totalbytes+=toberead;
         //buffo+=28;

         //printf("   from rack %d\n",theCrate);
         //argstr->totbytes = ((long)buffo - (long)datab);
         //argstr->totbytes=totalbytes;
         printf("STAT %x %d : %x %x \n",*evh,htonl(*(evh+5)),*(evh+1),*(evh+2));
        printf("    %x %x %x %x %x %x %x\n",*evh,htonl(*(evh+1)),
               *(evh+2),htonl(*(evh+3)),htonl(*(evh+4)),
                         htonl(*(evh+5)),htonl(*(evh+6)));
         if ((evh[1]==0) && (evh[2]==0))
         {
         }
        }//end STAT
       else if (strncmp(buffo,"DATA",4)==0)
       {
         totalfrag++;
         //if ((totalfrag%100)==0)
         //printf( "Fragreceived %d %ld\n" ,totalfrag,totalbytes );
        //printf( "Fragreceived %d %ld\n" ,totalfrag,totalbytes );

       long *evh=(long *)buffo;
       long toberead= ntohl(evh[6]);
       //for (int ii=0;ii<7;ii++)
        //   printf ( " ev [%d] = %x \n ", ii, ntohl(evh[ii]));
       if ((toberead-28)>0)
        {
       //  printf("Receiving data packet (%ld bytes, %x id, %x abs time) \n",toberead,
        //   ntohl(evh[5]),ntohl(evh[4]));
        //int readnow =connP->Receive(&buffo[28],toberead-28);
        int readnow =read(mfd,&buffo[28],toberead-28);
         if (readnow!=(toberead-28) )
               printf ("Err: Lost data (%d instead %d)\n",readnow,toberead-28);
        }
         if(((ntohl(evh[5])&0xffff00)==0x210400)|| 
          ((ntohl(evh[5])&0xffff00)==0x220400)|| (ntohl(evh[4])==0x7f))
          {
         printf("Receiving data packet (%ld bytes, %x id, %x abs time) \n",toberead,
           ntohl(evh[5]),ntohl(evh[4]));
            for (int smp=0;smp<4096*8;smp+=(8*8))
              printf("%x ",ntohl(evh[7+smp])&0x3ff);
              printf("\n");
           }
         //buffo += toberead-28;
        //  write(fde,buffo,toberead);
          //totalbytes+=toberead;
       }
    else if (strncmp(buffo,"HKPN",4)==0)
     {
      printf("HKP received\n");
      //buffo-=28;
    }
    else if (strncmp(buffo,"EVEN",4)==0)
     {
      long *evh = (long *) buffo;
      time_t evti = ntohl (*(evh+3));
      printf("EVEN received  :\n");
      printf("  Run %d Ev %d at %s %x %x\n",ntohl(*(evh+1)),ntohl(*(evh+2)),
      ctime(&evti),ntohl(*(evh+4)),ntohl(*(evh+5)));
      //buffo-=28;
    }
    else
    {
      char toke[10];
      strncpy(toke,buffo,4);
      toke[5]=0;
       printf("%s: received \n",toke);
      //buffo-=28;
      
    }
     //buffo+=28;
   }//if packsize == 28
   else printf("size does not match 28-%d\n",rdsiz);
  } //while
 }
}
     



