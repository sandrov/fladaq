#include <stdlib.h>
#include <errno.h>
#include "Cicaevm.h"
#include "veto.cc"

#define kCheckConPer 100000000
#define kPort 4321
#define  kPortdata  4255
#define  kPortwrit  6789


class conchk : public icapertsk
{
   icaevm *mevm;
  public:
   conchk(icaevm* a):mevm(a),icapertsk(kCheckConPer){start();};
   void DoPerTask(){mevm->CheckConnections();};
};

class cmdin : public icathr
{
   icaevm *mevm;
  public:
   cmdin(icaevm* a):mevm(a){start();};
   void DoTask(){mevm->cmdloop();};
};


class daq : public icathr
{
    icaevm *mevm;
   public:
    daq(icaevm* a):mevm(a){};
    void DoTask(){mevm->daqloop();};
};

icaevm::icaevm()
{
   //mresbrow = new rbrow;
   mycmd = new keycmd;
   mdaq=new daq(this);
   gDone=0;
   evmdone=0;
   maxevs=1000;
}

icaevm::~icaevm()
{
   stop();
   delete mycmd;
   delete mdaq;
  // delete mresbrow;
}

void
icaevm::setCommonParameters(int trigs,int nsam,int pretr,int fsc,int ofs,int thr)
{
  trigsperfile=trigs;
  totalsamples=nsam;
  presamples=pretr;
  thresh=thr;
  offset=ofs;
  fullscale = fsc;
}

int 
icaevm::FetchCrateList()
{
   char hostname[255];
   char lineread[255];
  FILE *craf=fopen("rack.conf","r");
  
  totCrates=0;

  if (craf)
  {
    //while (fscanf(craf,"%s\n",&hostname)>0)
    while(fgets(lineread,255,craf))
     {

     if (lineread[0]=='#')
     {

     }
     else
     {
       sscanf(lineread,"%s",&hostname);
       printf("%d -- host %s \n",totCrates,hostname);

       struct hostent *hp = gethostbyname(hostname);

         if (hp == NULL) {
             (void) printf("host %s not found\n", hostname);
             exit (3);
         }
      char ** p = hp->h_addr_list;
      in_addr in;
      (void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));

      //crates[totCrates]=new icacrate(0);
      crates[totCrates]=new icacrate(in,&gstat);

      try
      {
       //mylog.WriteMsg("Connecting to  %s,%d ... \n", (int) inet_ntoa(in),(int)kPort);
       printf("Connecting to  %s,%d ... \n",inet_ntoa(in),kPort);

       printf("Cicaevm: fu %d ofs %d thre %d\n",fullscale,offset,thresh);
       crates[totCrates]->setCommonParameters(totalsamples,presamples,fullscale,offset,thresh);
       crates[totCrates]->initialize();
       totCrates++;
       }
      catch(...)
      {
       printf("errors with %s. Ignoring this crate!\n",hostname);
       //mylog.WriteMsg("errors with %s. Ignoring this crate!\n",(int)hostname);
      }
      }
     }
   }
}

int
icaevm::FetchWritList()
{
   char hostname[255];
  char lineread[255];

  FILE *craf=fopen("writ.conf","r");

  totWrit=0;
  if (craf)
  {
    //while (fscanf(craf,"%s\n",&hostname)>0)
    while(fgets(lineread,255,craf))
     {
   
     if (lineread[0]=='#')
     {

     }
     else
     {
       sscanf(lineread,"%s",&hostname);
       printf("%d -- host %s \n",totWrit,hostname);

      struct hostent *hp = gethostbyname(hostname);
         if (hp == NULL) {
             (void) printf("host %s not found\n", hostname);
             exit (3);
         }
      char ** p = hp->h_addr_list;
      in_addr in;
      (void) memcpy(&in.s_addr, *p, sizeof (in.s_addr));

      printf("Initializing writ %d therun %d totcrates%d\n",totWrit,theRun,totCrates);
      icawrihdl *mp=new icawrihdl(totWrit,theRun,totCrates);
      printf(" writ handl created %d therun %d totcrates%d\n",totWrit,theRun,totCrates);
      writs[totWrit]=mp;
      //writstatus[totWrit]=1;


      printf("Initializing writ done\n");
      try
      {
       //mylog.WriteMsg("Connecting to  %s,%d ... \n",(int) inet_ntoa(in),(int)kPortwrit);
       printf("Connecting to  %s,%d ... \n",inet_ntoa(in),kPortwrit);
       writs[totWrit]->Connect(inet_ntoa(in),kPortwrit);
       int nh = htonl(theRun);
       writs[totWrit]->Send((char *)&nh,4);
       nh = htonl(totCrates);
       writs[totWrit]->Send((char *)&nh,4);
       nh = htonl(trigsperfile);
       writs[totWrit]->Send((char *)&nh,4);
       for (int aa=0;aa<totCrates;aa++) 
       {
	       nh=crates[aa]->mId.s_addr;
	       writs[totWrit]->Send((char *)&nh,4);
       }

       for (int aa=0;aa<100-totCrates;aa++) 
       {
	       nh=0;
	       writs[totWrit]->Send((char *)&nh,4);
       }

       totWrit++;
       }
      catch(...)
      {
       printf("errors with %s. Ignoring this writ!\n",hostname);
       //mylog.WriteMsg("errors with %s. Ignoring this writ!\n",(int)hostname);
      }
      }
     }
   }
}

void 
icaevm::configure()
{
  try {
     //mtrg = new icatrg;

     FetchCrateList();
    sleep(1);
     FetchWritList();
    sleep(1);
  }
  catch (...)
  {
  }
}

void
icaevm::start()
{
 configure();
 for (int theCrate=0;theCrate<totCrates;theCrate++)
 {
     crates[theCrate]->Send("strt",4);
 }
 mdaq->start();
}

void 
icaevm::stop()
{
 for (int theCrate=0;theCrate<totCrates;theCrate++)
 {
     crates[theCrate]->Send("stopquit",8);
 }
 for (int theWrit=0;theWrit<totWrit;theWrit++)
 {
     writs[theWrit]->Send("quit0000",8);
 }
 mdaq->stop();
}

void 
icaevm::CheckConnections()
{
  int theCrate;
  int theWrit;

    for(short  bb=0;!evmdone;bb++)
     {
       sleep(1);
       try
       {
         for (theCrate=0;theCrate<totCrates;theCrate++)
         {
            crates[theCrate]->Send("noop",4);
         }
       }
       catch(...)
       {
           printf("Crate #%d disappeared (err %d)!!! Closing down.\n",
                     crates[theCrate]->Id(),errno);
           //mylog.WriteMsg("Crate #%d disappeared (err %d)!!! Closing down.\n",
           //          crates[theCrate]->Id(),errno);
           gDone=-1;
       }
       try
       {
         for (theWrit=0;theWrit<totWrit;theWrit++)
         {
            writs[theWrit]->Send("uhuh",4);
         }
       }
       catch(...)
       {
          printf("Writer #%d has troubles!!! Closing down.\n",
                 writs[theWrit]->Id());
          //mylog.WriteMsg("Writer #%d has troubles!!! Closing down.\n",
          //        writs[theWrit]->Id());
           gDone=-2;
       }
    }
}

icawrihdl *
icaevm::selectWriter()
{
  if (totWrit==0) return 0;
  int writ=0;

  int timeout=1;
  do
  {
    if (writs[writ]->mStatus==0) return writs[writ];
    writ++;
    if (writ>totWrit) writ=0;
  } while (timeout);

  return 0;
}

void 
icaevm::cmdloop()
{
  while (!evmdone)
  {
    switch(mycmd->GetNextCommand())
    {
      case CMD_STOP:
          stop();
          evmdone = 1;
          sleep(2);
          break;

      case CMD_STATUS:
	 printstatus();
         break;
	 
      default: 
	 ;
     }
  }

}

void
icaevm::printstatus()
{
	printf(" Total events %d, bandwidth %d \n",gstat.totalevs,
			gstat.bandwidth);

}

void 
icaevm::daqloop()
{
  printf("Starting Daq %d evs\n",maxevs);
  int totevs = 0;
   while ((!evmdone) && ((totevs<maxevs)||(!maxevs)))
   {
     totevs++;

     icawrihdl * mwri=selectWriter();  

     trip thetr;
    // thetr.evid = mtrg->waitTrig();
     vetoOff();
     thetr.evid = totevs;
     thetr.when=time(0);
     thetr.type=trigmode;

     //if (!(totevs%100) ) printf("Trig #%d\n",totevs);
      printf("Trig #%d\n",totevs);

     for (int crate=0; crate<totCrates;crate++)
         crates[crate]->tellTrig(thetr.evid);
#ifdef DEBUG
     printf("Trigger sent to crates.\n");
#endif
     if(mwri)
       mwri->spawnReceive(&thetr);
     else printf("No writer...\n");
   }
   stop();
}
