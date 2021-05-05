#include "Ctcpcom.h"
#include "Cicathr.h"

class flaCrate;

class CmdListener : public icathr, tcpcom
{
    private:
        bool alive;
        char combuf[100];
        Ctcp* conn;
        flaCrate* ownerCr;
        void parseCommand ();
    
    public: 
        CmdListener (flaCrate* owner, Ctcp* conn0);
        ~CmdListener ();
        
        virtual void DoTask();
        
};
