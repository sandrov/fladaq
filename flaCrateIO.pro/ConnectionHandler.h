#ifndef _ConnectionHandler_
#define _ConnectionHandler_

#include "Ctcpux.h"

// Manages the TCP connection.

class ConnectionHandler
{
    private:
        int port;
        Ctcp* conn;
        
    public: 
        ConnectionHandler(int port0); 
        ~ConnectionHandler();
        
        Ctcp* getConnection(); // Waits a connection on a port and returns it when connected.
};

#endif
