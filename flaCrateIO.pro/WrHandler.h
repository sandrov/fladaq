#include <string.h>
#include <errno.h>

#include "DataManager.h"
#include "Ctcpux.h"
#include "Cicathr.h"

// Handles the communication with a Writer on the port passed to the constructor.
class WrHandler : public icathr
{
    private:
        DataManager* dataSrc;
        Ctcp* wrConn; // TCP connection with the Writer
        
        void waitRequest (void); // Waits the request for an event
        void sendEvent (void); // Sends the event to the Writer
        void sendStat (void); // Sends the STAT to the Writer
        
    public:
        WrHandler (DataManager* dSrc, Ctcp* conn);
        virtual ~WrHandler (void); 

        virtual void DoTask();

};
