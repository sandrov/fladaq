#include "Cicaevrecv.h"
#include <stdio.h>
#include "packs.h"
#include <string.h>

#define DEBUG 1
#define kHeadSize 28

evrecv::evrecv(icacrate *thecrate, buistat *thestats)
{
      mcrate= thecrate;
      gstat = thestats;
  
}

evrecv::~evrecv()
{
}


void 
evrecv::DoTask()
{
  totbytes=0;
  char *buffo=datab;
  mcrate->Send("getevent",8);
#ifdef DEBUG
  printf("getevent sent to crate %x\n", mcrate);
#endif
  for (;;)
  {
#ifdef DEBUG
     printf("Waiting for header from crate %x...\n", mcrate);
#endif
     if (mcrate->Receive(buffo,28) == kHeadSize) 
     {
#ifdef DEBUG
         printf("Header received correctly from crate %x\n", mcrate);
#endif
         int *evh=(int *)buffo;
         int toberead= ntohl(evh[6]);
         
#ifdef DEBUG
         printf("%d bytes to be read from crate %x\n", toberead-kHeadSize, mcrate);
#endif
         
         if ((toberead-kHeadSize)>0)
         {
#ifdef DEBUG
	   printf("Reading now %d bytes from %x crate.\n", toberead-kHeadSize, mcrate);
#endif
           int readnow = mcrate->Receive(&buffo[kHeadSize],toberead-kHeadSize);
#ifdef DEBUG
	   printf("Received now %d bytes from %x crate.\n", readnow, mcrate);
#endif
           if (readnow!=(toberead-kHeadSize) )
           {
             printf ("Err: Lost data (%d instead %d)\n",
                                       readnow,toberead-kHeadSize);
             return;
           }
         }
        
         if (strncmp(buffo,"STAT",4)==0)
         {
           buffo+= toberead;     //keep on buffer only STAT & DATA
           totbytes += toberead;
           gstat->totalstat++;
#ifdef DEBUG
           if ((gstat->totalstat%100)==0) 
              printf( "statreceived %d\n" ,gstat->totalstat );
#endif
              
          statpack * thepack = (statpack *) buffo; 

#ifdef DEBUG
	  printf("Got STAT pack from crate %x\n", mcrate);
#endif
          
	printf("Crate %x memstat1 %x memstat2 %x \n", mcrate,thepack->memstat1,thepack->memstat2);
        return; // This was put there to avoid blocking because of lost triggers in one board
          if (!((ntohl(thepack->memstat1)) | (ntohl(thepack->memstat2))))
	  {
#ifdef DEBUG
		printf("Crate %x returning\n", mcrate);
#endif
           return; }
         }  
         else if (strncmp(buffo,"DATA",4)==0)
         {
            buffo+= toberead;     //keep on buffer only STAT & DATA
            totbytes += toberead;

            gstat->totalfrag++;
#ifdef DEBUG
            if ((gstat->totalfrag%100)==0)
            printf( "Fragreceived %d %ld\n" ,gstat->totalfrag,totbytes );
#endif
            
#ifdef DEBUG
            printf("Got DATA pack of crate %x.\n", mcrate);
#endif
         } 
         else if (strncmp(buffo,"HKPN",4)==0)
         {
            /* ishould copy the hkp into crate object data */
         }
         else 
         {
               char toke[10];
               strncpy(toke,buffo,4);
               toke[5]=0;
               printf("%s: received \n",toke);
         }

         if (totbytes > kMaxEvSize - kMaxPackSize)
         {
             printf ("Err: Max buffer size reached, exiting");
             return;
         }
    }
    else 
    {
      printf ("Err: receiving from crate %d, exiting\n",mcrate->Id());
      return;
    }
 }
}

