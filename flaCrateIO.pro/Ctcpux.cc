#include "Ctcpux.h"
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#ifdef __APPLE__
//typedef int socklen_t;
#endif

Ctcp::Ctcp()
{
   Ctcp(0);
}

Ctcp::Ctcp(int localport)
{
//  struct sockaddr_in myaddr;
 
    connected=false;

     printf("Ctcp::Ctcp(): zeroing...\n");
    bzero ((char *) &myaddr, sizeof(myaddr));
     printf("Ctcp::Ctcp(): zeroing done..\n");


   sock = socket (AF_INET, SOCK_STREAM, 0);
     printf("Ctcp::Ctcp(): create socket..\n");

   if (sock < 0)
   {
     printf("Ctcp::Ctcp(): no socket...\n");
      throw tcpExcp(errno);
   }
  

    myaddr.sin_family       = AF_INET;
    myaddr.sin_port         = htons (localport);
 
    //int blen = 65536;
    int blen = 65536*8;

     printf("Ctcp::Ctcp(): setting socket opts buf...\n");
//    if(setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *)&blen,sizeof(blen))<0)
//      throw tcpExcp(errno);
//    if(setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *)&blen,sizeof(blen))<0)
//      throw tcpExcp(errno);
     printf("Ctcp::Ctcp(): setting socket opts reuse...\n");
    if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,(char *)&blen,sizeof(blen))<0)
      throw tcpExcp(errno);
     printf("Ctcp::Ctcp(): setting socket opts nodelay...\n");
//    if(setsockopt(sock,SOL_SOCKET,TCP_NODELAY,(char *)&blen,sizeof(blen))<0)
//      throw;
     printf("Ctcp::Ctcp(): binding...\n");

    port = localport;

    if (port)
        if(bind(sock,(struct sockaddr *)&myaddr,sizeof (myaddr)) < 0)
        { perror ("bind failed");
          throw tcpExcp(errno);
        }

          
}

Ctcp::Ctcp(int snew, int opt)
{

   connected = true;
   port =0;

   sock = snew;
}

Ctcp::Ctcp(Ctcp* aconn)
{

   connected = aconn->connected;
   port = aconn->port;

   sock = aconn->sock;
}

Ctcp::~Ctcp()
{
  if (connected) shutdown(sock,2);
  close (sock);
}

short 
Ctcp::Id()
{
    long maddr = clientAddr.sin_addr.s_addr;
    maddr = htonl (maddr);
    return maddr&0xff;
}

unsigned long
Ctcp::addr()
{
    unsigned long maddr = clientAddr.sin_addr.s_addr;
    maddr = htonl (maddr);
    return maddr;
}

int
Ctcp::Disconnect()
{
  connected = false;
  return shutdown(sock,2);
}

Ctcp *
Ctcp::Accept()
{

 //   struct sockaddr_in  clientAddr; /* client's address */    

    bzero ((char *) &clientAddr, sizeof (clientAddr));

    if (listen (sock, 2) < 0)
        {
        perror ("Ctcp::Accept(): listen failed");
        throw tcpExcp(errno);
        }

    int len = sizeof (clientAddr);

    int snew = accept (sock, (struct sockaddr *) &clientAddr,(socklen_t *) &len);
    if (snew ==0) 
    {
        perror ("Ctcp::Accept(): accept failed");
        throw tcpExcp(errno);
    }
 
    return new Ctcp(snew,0);
}

void 
Ctcp::Connect(unsigned long in,int toport)
{
    clientAddr.sin_family      = AF_INET;
    clientAddr.sin_addr.s_addr = htonl (in);
    clientAddr.sin_port        = htons (toport);
  
 if (connect (sock, (struct sockaddr  *)&clientAddr, sizeof (clientAddr)) < 0)
{
        perror ("Ctcp::Connect(): connect failed " );
        throw tcpExcp(errno);
}
  connected = true;
}

void 
Ctcp::Connect(char *host,int toport)
{
    clientAddr.sin_family      = AF_INET;
    clientAddr.sin_addr.s_addr = inet_addr (host);
    clientAddr.sin_port        = htons (toport);
  
 if (connect (sock, (struct sockaddr  *)&clientAddr, sizeof (clientAddr)) < 0)
{
        perror ("Ctcp::Accept(): connect failed");
        throw tcpExcp(errno);
}
  connected = true;
}

int
Ctcp::WaitData(int timeout)
{
//    return  read (sock, buffer,size) ;
     fd_set rfds;
     struct timeval tv;
   
    FD_ZERO(&rfds);
    FD_SET(sock,&rfds);

    tv.tv_sec = timeout;
    tv.tv_usec = 0;
 
     int retva = select (1,&rfds,0,&rfds,&tv);
     if (retva)
         if (FD_ISSET(0,&rfds)) return 1;
         else return -1;
     else return 0; 
}

int
Ctcp::Receive(char *buffer,int size)
{
//    return  read (sock, buffer,size) ;
 
    int howmany = 0;
    int toberead = size;
    do
    {
      //int readnow = recv (sock, &buffer[howmany], toberead,MSG_WAITALL) ;
      int readnow = recv (sock, &buffer[howmany], toberead,0) ;
      //if (readnow < 0 ) {printf("some errors...%d\n",errno); return -1;}
      if (readnow <= 0 ) 
           {printf("Ctcp::Accept(): some errors...%d\n",errno); throw tcpExcp(errno);}
      else { 
        howmany+=readnow; toberead-=readnow;}
    } while (toberead>0);
    return howmany;
}

int
Ctcp::Send(char *buffer,int size)
{
  if (connected==false) throw tcpExcp(EPIPE);
    int myret =  write (sock, buffer, size) ;
    if (myret<0) throw tcpExcp(errno);
    return myret;
}
