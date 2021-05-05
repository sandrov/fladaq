#include "WrHandler.h"
#include <sys/time.h>

WrHandler::WrHandler (DataManager* dSrc, Ctcp* conn) : dataSrc(dSrc), wrConn(conn) 
{
    printf("Here's the WrHandler...\n");
    this->start();
}

WrHandler::~WrHandler (void) 
{
    delete wrConn;
    delete dataSrc;
    
}

void
WrHandler::DoTask (void) 
{
    int i = 1;
    while (true) {	
        waitRequest();
        if (!(i % 100))   
		printf("Progression number: #%d\n", i);
        sendEvent();
	//printf("Event sent. Progression number: #%d\n", i);
        sendStat();
        dataSrc->ArmTrigger();
	i++;
    } // while
}

void 
WrHandler::waitRequest (void)
{
    const int size = 8;
    char buf[size+1];
    buf[size] = '\0';
    int read;
    bool done = false;
    do 
    {
        try
        {
#ifdef DEBUG
            printf("WrHandler::waitRequest(): waiting request...\n");
#endif
            read = wrConn->Receive(buf, size);
        }
        catch (...)
        {
            perror("WrHandler::WaitRequest()");
        }
        if (read != size) {
            printf("WrHandler::waitRequest(): Warning: size of message seems to be wrong.");
	    throw tcpExcp(errno); }
            
#ifdef DEBUG
        printf("WrHandler::waitRequest(): Received message: %s.\n", buf);
#endif
        
        if (strcmp(buf, "getevent")) {
            printf("WrHandler::waitRequest(): Wrong message\n");
	    throw tcpExcp(errno); }
        else {
            done = true;
#ifdef DEBUG
            printf("WrHandler::waitRequest(): got request.\n"); 
#endif
             }
    } while (!done);
} // waitRequest

void
WrHandler::sendEvent (void)
{
  dataSrc->waitData();

  while (dataSrc->dataAvail())
   {
    DataTile* tile = dataSrc->getData();
    char* data = (char*)tile;
//#ifdef _dbg_

    printf("WrHandler::sendEvent(): Sending event, size: %d\n", ntohl(tile->Header.packSize));
//#endif

    wrConn->Send(data, ntohl(tile->Header.packSize));
    
#ifdef _dbg_
    printf("WrHandler::sendEvent(): Event sent.\n");
#endif
    //delete tile;
   }

}

void
WrHandler::sendStat (void)
{
    statpack* stat = dataSrc->getStat();
    char* data = (char*)stat;
    
#ifdef _dbg_
    printf("WrHandler::sendEvent(): Sending stat.\n");
#endif

    wrConn->Send(data, sizeof(statpack));
    
#ifdef _dbg_
    printf("WrHandler::sendEvent(): Stat sent.\n");
#endif

    //delete stat;
}
        
