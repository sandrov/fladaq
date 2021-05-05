#include "Ccomma.h"
#include "Ctcpux.h"

#define MSG_QUIT 500
#define MSG_STATUS 401
#define MSG_DEDP   410
#define MSG_TRGP   411
#define MSG_MODE   322 
#define MSG_INIT   301 
#define MSG_FLSH   240 
#define MSG_STRT   220 
#define MSG_STOP   210 
#define MSG_NONE 0

using std::cout;
using std::endl;

class tcpcom: public comma
{
     Ctcp *mPort;

     public:
         tcpcom(Ctcp *);
         ~tcpcom();

     virtual int getnextcommand(char *combuf,short *cmdlist,short *ncmd);
};
