#include "Cicacrate.h"
#include "Cicaevrecv.h"
#include "Cicaconfdb.h"
#include "msgs.h"
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define kDataPort 4255
#define kPort 4321

icacrate::icacrate(in_addr raddr,buistat *gstat ):Ctcp(0)
{
  status = 0;
  mrcv = new evrecv(this,gstat);
  mId = raddr; 
}

icacrate::icacrate(int raddr,buistat *gstat ):Ctcp(0)
{
  status = 0;
  mrcv = new evrecv(this,gstat);
  mId.s_addr = raddr; 
}

icacrate::~icacrate()
{
  delete mrcv;
}

int 
icacrate::dataconnect()
{
    printf("Connecting to %s,%d\n",inet_ntoa(mId),kDataPort);
	Connect(htonl(mId.s_addr),kDataPort);
	return 0;
}

void
icacrate::setCommonParameters(int tot,int pre, int fsc,int ofs,int thre)
{
  totsam=tot;
  npre=pre;
  thres=thre;
  offset=ofs;
  fullscale=fsc;
}

int 
icacrate::initialize()
{
   initpar inipa;
   dedpar m;
   trigpar t;
   int Ids=0;
   
   char fname[80];
   sprintf(fname,"icab%d.par",((int)inet_lnaof(mId))&0xff);
 try
  {
    printf("Icacrate : Connecting to %s,%d,Parf %s\n",inet_ntoa(mId),kPort,fname);
    Connect(htonl(mId.s_addr),kPort);

    FILE *parf=fopen(fname,"r");

  if (parf)
   {
     char dum[10];
     printf("Crate %x - Reading parm's...\n", inet_ntoa(mId));

     fscanf(parf,"%s %hd",dum, &inipa.crateID);
   
     fscanf(parf,"%s %hd",dum, &inipa.firstboard);
     fscanf(parf,"%s %hd",dum, &inipa.last);

    //inipa.crateID = htons(inipa.crateID);
    inipa.crateID = htons(this->Id());
    inipa.firstboard = htons(inipa.firstboard);
    inipa.last = htons(inipa.last);

    inipa.token=htonl('init');
    this->Send((char*)&inipa,sizeof(initpar));

     printf("first %d last %d\n",inipa.firstboard ,inipa.last);

     fscanf(parf,"%s %hd",dum, &t.mode);
     fscanf(parf,"%s %hd",dum, &t.trigmode);
     fscanf(parf,"%s %hd",dum, &t.npre);
     fscanf(parf,"%s %hd",dum, &t.npost);
     fscanf(parf,"%s %hd",dum, &t.lat);
     
     icaconfdb myconf(this->Id());
     myconf.trgp_insert(&t);

     //t.mode = htons(t.mode);
     //t.trigmode = htons(t.trigmode);
     //t.npre = htons(t.npre);
     t.mode = htons(totsam);
     t.trigmode = htons(npre);
     t.npre = htons(t.npre);
     t.npost= htons(t.npost);
     t.lat  = htons(t.lat );



     printf("trig mode %d\n",t.mode);


     t.token=htonl('trgp');
     //this->Send((char*)&t,sizeof(trigpar));
     this->Send((char*)&t,18);
     printf("TRGP message sent.\n");
					  

   while(  fscanf(parf,"%s %x",dum, &m.mask1)!=EOF)
   {
     fscanf(parf,"%s %x",dum, &m.mask2);
     fscanf(parf,"%s %x",dum, &m.chans);
     fscanf(parf,"%s %hd",dum, &m.thre);
     fscanf(parf,"%s %hd",dum, &m.fullscale);
     for (int ii=0; ii<16; ii++)
     {
     	fscanf(parf,"%s %hd",dum, &m.coff[ii]);
     	fscanf(parf,"%s %hd",dum, &m.cthre[ii]);
     }

     myconf.insert();
     myconf.fep_insert(&m);


     m.mask1 = htonl (m.mask1);
     m.mask2 = htonl (m.mask2);
     m.chans = htonl (m.chans);
     //m.rsum = htons (m.rsum);

     printf("Fullscale %d Offset %d Thre %d\n",fullscale,offset,thres);
     m.thre = htons (thres);
     m.fullscale = htons (fullscale);
     m.offset = htons (offset);
     for (int ii=0; ii<16; ii++)
     {
       m.coff[ii]= htons (m.coff[ii]);
       m.cthre[ii]= htons (m.cthre[ii]);
     }

     m.token=htonl('dedp');
     this->Send((char*)&m,sizeof(dedpar));
     printf("DEDP message sent.\n");
    }

    fclose(parf); 

     }

   else
   {
     inipa.token=htonl('init');
     inipa.firstboard=htons(4);
     inipa.last=htons(6);
     //Ids++;
     inipa.crateID = htons(this->Id());

     this->Send((char*)&inipa,12);

   }

  return 0;
 } //try
 catch (...)
  {
   printf("errors with %s. Ignoring this crate!\n",hostname);
   return -1;
  }
}

void
icacrate::tellTrig(int evid)
{
  char mbu[8];
  int *id = (int *) &mbu[4];
  strcpy(mbu,"mode");
  *id=ntohl(evid);
  Send(mbu,8);
}
