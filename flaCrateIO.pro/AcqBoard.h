#include <unistd.h>
#include <stdio.h>

#include "AcqirisImport.h"
#include "AcqirisD1Import.h"

#include "AcqUtil.h"
#include "structures.h"

#ifndef _AcqBoard_
#define _AcqBoard_

static const char firmware[] = "AC240.bit";

class AcqBoard
{
    private:
        int boardNbr;
        long serialNbr,busNbr,slotNbr;
        ViChar boardname[256];
        ViSession boardId; 
        int preSamples;
        int mThre;
        
        long write(long reg, long n, long* buffer);
        long read(long reg, long n, long* buffer);
        
    public:
        int nSamples;

        AcqBoard(int nbr, ViSession id);
        ~AcqBoard();
        void configure(BoardConf conf);
        void configureTrig(TrigConf conf);
        void startDPU();
        int isDataRdy();
        int getData(int channel, char* buf);
        int ArmTrigger();
        int fillHeader(DataTile *);
};

#endif
