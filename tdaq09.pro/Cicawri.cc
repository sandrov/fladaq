#include "Cicawri.h"
#include "Cicaevrecv.h"
#include "packs.h"
#include <string.h>

icawri::icawri(icaevmhdl *cEvm,actimon *cActi):mEvm(cEvm)
{
  //mActi = new actimon;
  mActi = cActi;
  notDone=1;
}


icawri::~icawri()
{
 delete mActi;
 for (int crate =0;crate<totCrates;crate++)
  delete mCrateList[crate];
}

void 
icawri::Configure()
{
   totCrates=0;
  printf("Configuring %d crates\n",totCrates);  

   for (int crate=0;crate<mEvm->wpa.nocrates;crate++)
   {
      printf("Configuring crate %d %x\n",crate,(int)mEvm->wpa.crateid[crate]);  
     icacrate *newcrate = new icacrate( (int)htonl(mEvm->wpa.crateid[crate]),&mStat);
     if (newcrate->dataconnect()) delete newcrate;
     else 
     {
        mCrateList[totCrates++]=newcrate;
     }
   }
}

void 
icawri::WriteHeader()
{
  evHead mhea;

  mStat.totalevs++;

  mhea.token= htonl('EVEN');
  //mhea.token= htonl('TRIG');
  mhea.Run= htonl(mEvm->wpa.runno);
  mhea.Event = htonl(evId);
  mhea.ToD = htonl((int)time(0));
  mhea.AbsTime = htonl(mEvm->mtr.when);
  mhea.Conf = htonl((mEvm->mtr.type<<16)+totCrates);
  mhea.Size= htonl(sizeof(evHead));

#ifdef DEBUG
  printf ("Writing header...\n");
#endif
  mfil->Write((char *)&mhea,sizeof(evHead));
#ifdef DEBUG
  printf ("Written header...\n");
#endif
}


void
icawri::DoTask()
{
  mEvm->handleInit();
  Configure();
  
  try
  {
   mfil=new icadskfil(mEvm->wpa.runno,1000000000,1);
   mfil->opennewfile();
   while(notDone)
   {
     mActi->Status(waittrig,mfil->mpresdisk,mStat.totalevs,mfil->takeno,
           mStat.bandwidth);

     evId=mEvm->waitTrig(); 
    
#ifdef DEBUG
     printf("Trig #%d - Trying to receive\n",evId);
#endif
     WriteHeader();

     mActi->Status(receiving);
     //spawn receiving
     for (int crate=0;crate<totCrates;crate++)
     {
#ifdef DEBUG
      printf("starting crates\n");
#endif
      mCrateList[crate]->mrcv->start();
     }

     //synchronize 
     for (int crate=0; crate<totCrates; crate++)
    { 
        
        mCrateList[crate]->mrcv->join(); 
    }

     mEvm->receivingDone(); 

     mActi->Status(writing);

     //dump ev fragments on file
     for (int crate=0;crate<totCrates;crate++)
      mfil->Write(mCrateList[crate]->mrcv->datab,
                    mCrateList[crate]->mrcv->totbytes);
       
     if (!(evId%(mEvm->wpa.evsperfile)))
     {
        mfil->moveToDone(evId-mEvm->wpa.evsperfile);
/*     if( mfil->moveToDone(evId))
      {
*/
        while (mfil->checkSpace())
         {
              printf ("waiting disk space...\n");
              mActi->Status(waitdisk);
              //mEvm->giveStatus(mActi->mbrd.status);
              sleep(30);
         }
        mActi->Status(waitdisk);
        mfil->opennewfile();
        mActi->Status(0,mfil->mpresdisk);
      }
     
     //ready for next event
     //mEvm->giveStatus(mActi->mbrd.status);

    } //while

   mlog.Tell("Writer closing.");
   }  //try
  catch (...)
  {
    mlog.Warn("Writer closing.");
  }
   mfil->moveToDone(evId);
   mEvm->giveUp();

   for (int crate=0;crate<totCrates;crate++)
   {
     delete mCrateList[crate]; 
   }
}

