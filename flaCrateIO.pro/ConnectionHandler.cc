#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler (int port0) : port(port0), conn(new Ctcp(port0)) {}

ConnectionHandler::~ConnectionHandler (void) {}

Ctcp* 
ConnectionHandler::getConnection (void) 
{
    bool connected = false;
    Ctcp* newConn;
    //printf("ConnectionHandler::getConnection(): Waiting connection on port %d.\n", port);
    while (!connected) 
    {   
        
        try 
        {
            newConn = conn->Accept();
            connected = true;
        }
        catch (...) {}
    }
    //printf("ConnectionHandler::getConnection(): Connected.\n");
    return newConn;
} // getConnection
            
        

    
