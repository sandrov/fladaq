#include "flaCrate.h"
#include "CmdListener.h"

flaCrate::flaCrate (void) : evmConn(NULL), cmdIn(NULL)
{
    physCr = new PhysCrate();
    physCr->initialize();
    
    dataMgr = new DataManager(physCr);
    printf("flaCrate::flaCrate(): flaCrate created and configured.\n");
    
}

flaCrate::~flaCrate() 
{
    delete evmConn;
    delete dataMgr;
}

void
flaCrate::getEvmConnection (void)
{
    printf("flaCrate::getEvmConnection(): Waiting connection from EVM.\n");
    ConnectionHandler* connHdl = new ConnectionHandler(evmPort);
    evmConn = connHdl->getConnection();
    printf("flaCrate::getEvmConnection(): Connection with EVM successful.\n");
    delete connHdl;
}

void
flaCrate::spawnCmdListener (void)
{
    cmdIn = new CmdListener(this, evmConn);
}

void
flaCrate::spawnWrManager (int port)
{
    WrManager* wmgr = new WrManager(port, dataMgr);
    wmgr->start();
}

void
flaCrate::waitStopCmd (void)
{
    cmdIn->join();
}

void
flaCrate::setInitParams(initpar inpr)
{
    params = inpr;
    printf("Init parameters set. Size of initpar: %d\n", sizeof(initpar));
}

void
flaCrate::configCrate(BoardConf conf)
{
    physCr->configure(conf);
}

void
flaCrate::configTrigger(TrigConf conf)
{
    physCr->configureTrig(conf);
}

void
flaCrate::start()
{
    physCr->start();
}

void
flaCrate::ArmTrigger()
{
    //physCr->ArmTrigger();
}

