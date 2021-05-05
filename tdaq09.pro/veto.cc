#include <string.h>
#include <stdio.h>
#include "Cudpux.h"

Cudp mytrig(8888);
Cudp mysock(10001);

int totp=0;
//extern "C"
int vetoOn()
{
  char mhost[128];
  char buf[128];
  char myrec[256];
  char st[256];
  int retval=0;

  strcpy(mhost,"192.168.2.152");
//  strcpy(mhost,"172.16.4.169");
  strcpy(buf,"12on");

  do {
    sprintf(st,"%2d",totp++%100);
    strncpy(buf,st,2);
    mytrig.SendTo(mhost,10000,buf,14);  
    retval = mysock.Receive(myrec,2,1); 
    //retval = mysock.Receive(myrec,2); 
//    printf("sent received %s %s\n",buf, myrec);
    if (retval<0) printf("Lost udp packet\n");
  } while (retval<0);
   
//   printf("veto ON\n");
return 0;
}

//extern "C"
int vetoOff()
{
  char mhost[128];
  char buf[128];
  char myrec[256];
  int retval=0;

  //strcpy(mhost,"192.168.2.165");
  strcpy(mhost,"192.168.2.152");
//  strcpy(mhost,"172.16.4.169");
  strcpy(buf,"12of");

  do {
    mytrig.SendTo(mhost,10000,buf,14);  
    //retval = mysock.Receive(myrec,2,1); 
    //retval = mysock.Receive(myrec,2); 
    if (retval<0) printf("Lost udp packet\n");
  } while (retval<0);

 //  printf("veto OFF\n");
return 0;
}

