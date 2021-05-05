#ifndef _WrManager_
#define _WrManager_

#include <string.h>
#include <errno.h>

#include "ConnectionHandler.h"
#include "DataManager.h"
#include "Ctcpux.h"
#include "Cicathr.h"
#include "WrHandler.h"

// Handles the incoming connections from the writers.
class WrManager : public icathr
{
    private:
        int port;
        DataManager* dataSrc;
        
    public:
        WrManager (int port, DataManager* dSrc);
        virtual ~WrManager (void); 

        virtual void DoTask();
};

#endif
