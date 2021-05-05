#include <stdio.h>
#include <iostream>
#include "Ctcpux.h"
#include "WrManager.h"
#include "PhysCrate.h"
#include "msgs.h"

class CmdListener;

class WarpCrate 
{
    private:
        static const int evmPort = 4321; 
        static const int writerPort = 4255;
        
        Ctcp* evmConn; // TCP connection with Event Manager
        CmdListener* cmdIn; // Command Listener
        DataManager* dataMgr; // Data manager
        PhysCrate* physCr; // Physical crate handler
        initpar params; // Initialization parameters
    
    public:
        WarpCrate();
        ~WarpCrate();
        
        void getEvmConnection(); // Get connected with EVM
        void getWriterConnection(); // Get connected with Writer
        void spawnCmdListener(); // Spawns a command listener attached to the EVM connection   
        void spawnWrManager(int port = writerPort); // Spawns the writer manager.
        void waitStopCmd(); // Waits until a stop command is received.
        void setInitParams(initpar inpr);
        void configCrate(BoardConf conf);
        void configTrigger(TrigConf conf);
	void start();
        void ArmTrigger();
};
