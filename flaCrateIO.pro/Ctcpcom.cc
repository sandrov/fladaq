#include "Ctcpcom.h"
#include <string.h>
#include "msgs.h"

tcpcom::tcpcom(Ctcp *conn):mPort(conn)
{
  cout << "tcpcom::tcpcom(): Here's the commander.."<<endl;
}

tcpcom::~tcpcom()
{
  cout << "tcpcom::tcpcom(): Commander leaving" << endl;
}

int 
tcpcom::getnextcommand(char *combu,short *cmdlist,short* ncmd)
{
   //char *tok=new(nothrow) char[80];
   char *tok=new char[80];

   int aa = mPort->Receive(tok,4); 
   tok[4]=0;
//   fscanf(mPort->GetSocket(),"%s",tok);
   
#ifdef DEBUG
   printf("tcpcom::getnextcommand(): Got the command %s length %d\n", tok,aa);
#endif

   *ncmd=1;

   if (strncmp(tok,"qu",2)==0) {cmdlist[0]=MSG_QUIT; return MSG_QUIT;}
   else if (strncmp(tok,"QU",2)==0) {cmdlist[0]=MSG_QUIT; return MSG_QUIT;}
   else if (strncmp(tok,"stat",4)==0) 
        {cmdlist[0]=MSG_STATUS; return MSG_STATUS;}
   else if (strncmp(tok,"init",2)==0)
        {
           strncpy(combu,tok,4);
           mPort->Receive(combu+4,8);
           cmdlist[0]=MSG_INIT;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
            //printf("tok is %s myb is %d \n",tok,myb);
            //printf("cmdlist: %d %d \n",cmdlist[0],cmdlist[1]);
            //logMsg("the comm mode %x %x\n",myb,*(long *)combu);
           return MSG_INIT;
        }
   else if (strncmp(tok,"dedp",4)==0)
        {
           strncpy(combu,tok,4);
           mPort->Receive(combu+4,sizeof(dedpar));
           *ncmd=2;
           cmdlist[0]=MSG_DEDP;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_DEDP;
        }
   else if (strncmp(tok,"trgp",4)==0)
        {
           strncpy(combu,tok,4);
           mPort->Receive(combu+4,14);
           *ncmd=2;
           cmdlist[0]=MSG_TRGP;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_TRGP;
        }
   else if (strncmp(tok,"strt",4)==0)
        {
           *ncmd=1;
           cmdlist[0]=MSG_STRT;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_STRT;
        }
   else if (strncmp(tok,"flsh",4)==0)
        {
           *ncmd=1;
           cmdlist[0]=MSG_FLSH;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_FLSH;
        }
   else if (strncmp(tok,"mode",4)==0)
        {
           *ncmd=2;
           mPort->Receive(combu,4);
           // printf ("mode %d\n",*(short *)combu);
           cmdlist[0]=MSG_MODE;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_MODE;
        }
   else if (strncmp(tok,"stop",4)==0)
        {
           *ncmd=1;
           cmdlist[0]=MSG_STOP;
           cmdlist[1]=0;
           cmdlist[2]=0;
           cmdlist[3]=0;
           return MSG_STOP;
        }
    else return MSG_NONE;
} 
