#ifndef _DataManager_
#define _DataManager_ 

#include <string.h>
#include <stdio.h>

#include "PhysCrate.h"
#include "packs.h"
#include "structures.h"

// Manages the data flow of the WarpCrate
// In this version data are requested to the PhysicalCrate synchronously with 
// the requests of the WrHandler.

// A possible improvement could be the use of a thread-safe FIFO queue 
// with a thread which reads the data from the board as soon as there are
// ready data, independently from the transfer to the Writer. 

class DataManager
{
    private:
        PhysCrate* pCrate;
        
    public:
        
        static const int statSize = sizeof(statpack);
        
        DataManager (PhysCrate* cr);
        ~DataManager (void);

        void waitData();
        void ArmTrigger();
        bool dataAvail();
        
        DataTile* getData();

        statpack* getStat();

}; // DataManager

#endif
