#include "WarpCrate.h"
#include "CmdListener.h"

WarpCrate::WarpCrate (void) : evmConn(NULL), cmdIn(NULL)
{
    physCr = new PhysCrate();
    physCr->initialize();
    
    dataMgr = new DataManager(physCr);
    printf("WarpCrate::WarpCrate(): WarpCrate created and configured.\n");
    
}

WarpCrate::~WarpCrate() 
{
    delete evmConn;
    delete dataMgr;
}

void
WarpCrate::getEvmConnection (void)
{
    printf("WarpCrate::getEvmConnection(): Waiting connection from EVM.\n");
    ConnectionHandler* connHdl = new ConnectionHandler(evmPort);
    evmConn = connHdl->getConnection();
    printf("WarpCrate::getEvmConnection(): Connection with EVM successful.\n");
    delete connHdl;
}

void
WarpCrate::spawnCmdListener (void)
{
    cmdIn = new CmdListener(this, evmConn);
}

void
WarpCrate::spawnWrManager (int port)
{
    WrManager* wmgr = new WrManager(port, dataMgr);
    wmgr->start();
}

void
WarpCrate::waitStopCmd (void)
{
    cmdIn->join();
}

void
WarpCrate::setInitParams(initpar inpr)
{
    params = inpr;
    printf("Init parameters set. Size of initpar: %d\n", sizeof(initpar));
}

void
WarpCrate::configCrate(BoardConf conf)
{
    physCr->configure(conf);
}

void
WarpCrate::configTrigger(TrigConf conf)
{
    physCr->configureTrig(conf);
}

void
WarpCrate::start()
{
    physCr->start();
}

void
WarpCrate::ArmTrigger()
{
    //physCr->ArmTrigger();
}

