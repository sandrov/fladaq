#include "WrManager.h"
#include <sys/time.h>

WrManager::WrManager (int port0, DataManager* dSrc) : port(port0), dataSrc(dSrc) {}

WrManager::~WrManager (void) {}

void
WrManager::DoTask (void) 
{
    ConnectionHandler* chdl = new ConnectionHandler(port);
    while (true)
    {        
        printf("WrManager::DoTask(): Waiting connection from a writer...\n");
        Ctcp* c = chdl->getConnection();
        new WrHandler(dataSrc, c);
        printf("WrManager::DoTask(): Writer Handler spawned.\n");
    }
}

