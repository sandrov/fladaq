#ifndef _Cresoitem_
#define _Cresoitem_
#endif

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netdb.h>

class resoitem 

{
   //resoitem *startoflist;

   public: 

    in_addr resoAddr;

    char status[80];
    long resclass;
    long ttl;
    long resId;
    long par1,par2,par3,par4;

     resoitem *prev,*next;
     
    resoitem(resoitem** list);
    ~resoitem();
};

class resolist
{
  public:
   resoitem *thelist;
    resolist();
    ~resolist();
    void Addresoitem(in_addr resaddr,
             long newId,long newclass,char newstatus[80],
               long p1,long p2,long p3,long p4);

    int printlist();
    int ttlupdate();
};
