#ifndef _Ctcp_
#define _Ctcp_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>

class tcpExcp
{
     int errornumber;
  public:
   tcpExcp(int err):errornumber(err){};
};

class Ctcp
{
  protected:

    int port;
    int sock;

    int connected;
    struct sockaddr_in clientAddr;
    struct sockaddr_in myaddr;


  public:

    Ctcp();
    Ctcp(int port);
    Ctcp(int sock,int opt);
    Ctcp(Ctcp *);
    ~Ctcp();

    Ctcp * Accept();
    void Connect(char *hostaddr,int port);
    void Connect(unsigned long hostaddr,int port);
    int Disconnect();
   
    short Id();
    unsigned long addr();
    int Send(char * buffer,int size); 
    int Receive(char *buffer,int size);
    int WaitData(int timeout);

};

#endif
