#include <unistd.h>
#include <stdio.h>

#include "structures.h"

#include "CAENComm.h"
#include "A2795.h"

#ifndef _A2795Board_
#define _A2795Board_


class A2795Board
{
    private:
        int boardNbr;
        int serialNbr,busNbr,slotNbr;
        char boardname[256];
        int preSamples;
        int mThre;
        int bdhandle;
        int dacPed[4];
        
    public:
        
        int boardId; 
        int nSamples;

        A2795Board(int nbr, int id);
        ~A2795Board();
        void configure(BoardConf conf);
        void configureTrig(TrigConf conf);
        int readPedestals(int* peds);
        int writePedestals(int* peds);
        void startDPU();
        int isDataRdy();
        int getData(int channel, char* buf);
        int ArmTrigger();
        int fillHeader(DataTile *);
        int write(int reg, int buffer);
        int read(int reg, int* buffer);
};

#endif
