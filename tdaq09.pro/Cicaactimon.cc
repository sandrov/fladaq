#include "Cicaactimon.h"
#include <string.h>

void 
actimon::Status(int msta)
{
   mbrd.status=htonl(msta);

   switch (msta)
   {
    case 0:
     strcpy(mbrd.hurdstatus,"Ready");
     break;
    case waittrig:
     strcpy(mbrd.hurdstatus,"Waiting for trigger");
     break;
    case receiving:
     strcpy(mbrd.hurdstatus,"Receiving data");
     break;
    case writing:
     strcpy(mbrd.hurdstatus,"Writing data on disk");
     break;
    case waitdisk:
     strcpy(mbrd.hurdstatus,"Waiting for disk space");
     break;
    }
}

void 
actimon::Status(int which,int par1)
{
   mbrd.param[which]=htonl(par1);
}

void 
actimon::Status(int msta,int par1,int par2,int par3,int par4)
{
   Status(msta);
   mbrd.param[0]=htonl(par1);
   mbrd.param[0]=htonl(par2);
   mbrd.param[0]=htonl(par3);
   mbrd.param[0]=htonl(par4);
}
