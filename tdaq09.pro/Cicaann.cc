#include "Cicaann.h"
#include <string.h>

icaann::icaann(int port=4424,int mclass=1):Cudp(port)
{

  mbrd.mclass=htonl(mclass);
  mbrd.status=0;
  strcpy(mbrd.hurdstatus,"Ready");
}

icaann::~icaann()
{}

void 
icaann::DoTask()
{
  while(1)
  {
    SendTo("192.168.157.255",2244,(char *)&mbrd,256);
    sleep(1);
  }
} 
