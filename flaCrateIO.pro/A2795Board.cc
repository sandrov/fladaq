#include "A2795Board.h"
#include "structures.h"
#include "veto.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WRITE 1
#define READ 0

#define HOST 1



A2795Board::A2795Board(int nbr, int bus) : boardNbr(nbr), boardId(nbr) 
{
#ifndef _simulate_
// Acqrs_getInstrumentData(boardId, boardname, &serialNbr, &busNbr, &slotNbr);

   printf ("Trying BoardId %x serial %d bus %d slot %d\n",boardId,serialNbr, bus, nbr);
  int ret;
  ret=CAENComm_OpenDevice(CAENComm_OpticalLink, bus, nbr, 0, &bdhandle);
      if (ret != CAENComm_Success) boardId=-1;
     else 
      {
         int status;
         //CAENComm_Write32(bdhandle, A_Signals, SIGNALS_SWRESET);
         CAENComm_Read32(bdhandle, A_StatusReg,(uint32_t*) &status);
         printf("board %d status %d\n",boardId,status);
         boardId=status&STATUS_SLOT_ID;
         
        //CAENComm_Write32(bdhandle, A_ControlReg_Clear, CTRL_TTLINK_MODE);
//    CAENComm_Write32(bdhandle, A_ControlReg_Set, CTRL_ACQRUN);
      }
#else
#endif
 serialNbr=boardNbr;
 if (boardId!=-1) {	 
    printf ("BoardId %x serial %d bus %d slot %d\n",boardId,serialNbr, bus, nbr);

    FILE *stream;
    int mbus,slot;
    
    stream=fopen("dacpedestals.par", "r");
    if (stream==NULL) {
        printf("Error with dacpedestals.par");
    } else {
        char dummy[25];
        fscanf(stream, "%s %s %s %s %s %s\n",&dummy,&dummy,&dummy,&dummy,&dummy,&dummy);
        int i=0;
        while (i<10) {
            i++;
            fscanf(stream, "%d %d 0x%x 0x%x 0x%x 0x%x\n", &mbus,&slot, &dacPed[0], &dacPed[1], &dacPed[2], &dacPed[3]);
//            printf("board ped bus %d slot %d\n",mbus,slot);
            if ((mbus==bus)&&(slot==nbr)){
                //printf("board ped %d %d 0x%x 0x%x 0x%x 0x%x", dacPed[0], dacPed[1], dacPed[2], dacPed[3]);
                break;
            }
        }
    }
 
   fclose(stream);
 }
    	
}

A2795Board::~A2795Board()
{
  if (bdhandle!=-1)
  {
    CAENComm_Write32(bdhandle, A_Signals, SIGNALS_TTLINK_EOR);
    CAENComm_CloseDevice(bdhandle);
  }
}

#ifndef _simulate_

int
A2795Board::readPedestals(int* thePed)
{
            uint32_t data;
            CAENComm_Read32(bdhandle, A_DAC_A, &data);
            printf("DacA [ 0:31] Value %X Readback %X\n", dacPed[0],data);
            CAENComm_Read32(bdhandle, A_DAC_B, &data);
            printf("DacB [ 0:31] Value %X Readback %X\n", dacPed[1],data);
            CAENComm_Read32(bdhandle, A_DAC_C, &data);
            printf("DacC [ 0:31] Value %X Readback %X\n", dacPed[2],data);
            CAENComm_Read32(bdhandle, A_DAC_D, &data);
            printf("DacD [ 0:31] Value %X Readback %X\n", dacPed[3],data);
    

}

int
A2795Board::writePedestals(int* thePed)
{
            CAENComm_Write32(bdhandle, A_DAC_CTRL, 0x00000000 | (dacPed[0] & 0xFFFF) );
            CAENComm_Write32(bdhandle, A_DAC_CTRL, 0x00010000 | (dacPed[1] & 0xFFFF) );
            CAENComm_Write32(bdhandle, A_DAC_CTRL, 0x00020000 | (dacPed[2] & 0xFFFF) );
            CAENComm_Write32(bdhandle, A_DAC_CTRL, 0x00030000 | (dacPed[3] & 0xFFFF) );
            uint32_t data;
            CAENComm_Read32(bdhandle, A_DAC_A, &data);
            printf("DacA [ 0:31] Value %X Readback %X\n", dacPed[0],data);
            CAENComm_Read32(bdhandle, A_DAC_B, &data);
            printf("DacB [ 0:31] Value %X Readback %X\n", dacPed[1],data);
            CAENComm_Read32(bdhandle, A_DAC_C, &data);
            printf("DacC [ 0:31] Value %X Readback %X\n", dacPed[2],data);
            CAENComm_Read32(bdhandle, A_DAC_D, &data);
            printf("DacD [ 0:31] Value %X Readback %X\n", dacPed[3],data);

}


// Configures the board.
// Input (param): BoardConf conf - structure with the configuration parameters.
void
A2795Board::configure (BoardConf conf)
{		

  int status;


        printf( "Configuring Board \n");
 

       CAENComm_Write32(bdhandle, A_RELE, RELE_TP_DIS);


       //status = Acqrs_calibrate(boardId);
       //AcqUtils::checkStatus(status, "Calibrated.");




    // Configure timebase


 printf ( "Conf Board %d offs %f thre %f \n",boardNbr,conf.coff[boardNbr*2],
    conf.cthre[boardNbr*2]);
  printf ( "              offs %f thre %f \n",conf.coff[(boardNbr*2)+1],
    conf.cthre[(boardNbr*2)+1]);
 
        printf("off %f thre %f \n", conf.coff[(boardNbr)*2],conf.cthre[(boardNbr)*2]);
        printf("off %f thre %f\n", conf.coff[(boardNbr)*2+1],conf.cthre[(boardNbr)*2+1]);

       // Set DC offset
  //tobeset     CAENComm_Write32(bdhandle, A_DAC, 0x00070000 | (conf.coff[boardNbr*2] & 0xFFFF) );
        int ped[4];
        writePedestals(ped);

	printf("A2795Board::configure(): board %d configured.\n", boardNbr);
}

// Configures the trigger parameters.
// Input (param): TrigConf conf - structure with the trigger parameters.
void
A2795Board::configureTrig (TrigConf conf)
{

      nSamples=conf.nsamples;
      preSamples = conf.presamples;





      //printf("Set nSamples to %d \n",conf.nsamples);
      //printf("Set preSamples to %d \n",conf.presamples);
	//AcqrsD1_configTrigClass(boardId, conf.trigClass, conf.sourcePattern, 0, 0, 0.0, 0.0);
	//AcqrsD1_configTrigSource(boardId, conf.channel, conf.trigCoupling, conf.trigSlope, conf.trigLevel1, conf.trigLevel2);
  
    //    ViInt32 regValue;
    //    regValue=nSamples/16;
    //    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 71, 1, &regValue, WRITE, 0);

     //   regValue=24000/16; //pretrigger samples
     //   Acqrs_logicDeviceIO(boardId, "Block1Dev1", 72, 1, &regValue, WRITE, 0);
}

// Starts the DPU on the board.
void
A2795Board::startDPU ()
{

    printf("A2795Board::startDPU(): core of board (%d,%ld) started.\n", boardNbr, (int)boardId);
    CAENComm_Write32(bdhandle, A_ControlReg_Set, CTRL_ACQRUN);
//CAENComm_Write32(bdhandle, A_Signals, SIGNALS_TTLINK_SOR);

}

// Tells if data are ready
// Output (return): true if there are ready data, false otherwise.
int
A2795Board::isDataRdy()
{

     int timeoutCounter = 250000; // Timeout waiting for a trigger
     int errTimeoutCounter = 500000; // Timeout for acquisition completion
     //int errTimeoutCounter = 1; // Timeout for acquisition completion
#ifdef _dbg_
    printf("A2795Board::isDataRdy(): BoardId %d BoardNbr %d\n",boardId, boardNbr);
#endif

        int status;
        bool done=false;


	while (!done && errTimeoutCounter--)
	{
	//	AcqrsD1_acqDone(boardId, &done); // Poll for the end of the acquisition
        //      Acqrs_logicDeviceIO(boardId, "Block1Dev1", 69, 1, &regValue, READ, 0);
             CAENComm_Read32(bdhandle, A_StatusReg,(uint32_t*) &status);
             done=(status&STATUS_DRDY);  //has Data Ready in Status Reg
		if (!timeoutCounter--) // Trigger timeout occured
		{
			printf("A2795Board::isDataRdy(): Slow trigger...\n");
                       vetoOff();
		//AcqrsD1_forceTrig(boardId); // Force a 'manual' (or 'software') trigger
                }
	}
	return done;
}

int 
A2795Board::ArmTrigger()
{

#ifdef _dbg_
    printf("A2795Board::ArmTrigger(): wait a second...\n");
#endif


  return 0;
}

int
A2795Board::fillHeader(DataTile* buf)
{
  int  rValue[950*1024];
#ifdef _dbg_
    printf("A2795Board::fillHeader(): wait a second...\n");
#endif

     CAENComm_Read32(bdhandle, A_ControlReg,(uint32_t*)&buf->Header.info1);
     CAENComm_Read32(bdhandle, A_StatusReg,(uint32_t*)&buf->Header.info2);
     CAENComm_Read32(bdhandle, A_NevStored,(uint32_t*)&buf->Header.info3);
     //buf->Header.info3=((int)inet_lnaof(mId))&0xff;
//   buf->Header.timeinfo= htonl((int) rValue[0]);
//   buf->Header.chID= htonl(serialNbr);
}

#define BUFFER_SIZE ((4*1024)*32)+3

// Writes the data gathered by the board starting from the passed address.
// Output (param): char* buf - pointer to the memory area to write to.
int
A2795Board::getData(int channel,char* buf)
{


#ifdef _dbg_
    printf("A2795Board::getData(): wait a second...\n");
#endif

  // ### Readout the data ###
  int status,ret;
  int xco,ii,jj,totwords;
  int regValue,rValue[950*1024];
  char *mwaveform=(char *)buf;
  int nw;

     //  totwords=nSamples/4;

   ret = CAENComm_BLTRead(bdhandle, A_OutputBuffer,(uint32_t*) buf, BUFFER_SIZE, &nw);
   if ((ret != CAENComm_Success) && (ret != CAENComm_Terminated))
                printf("BLTReadCycle Error on Module (ret = %d)\n", ret);
 

for (int ii=0;ii<10;ii++)
     printf("%d ",rValue[ii]);
 printf("\n");
 printf("start decoding nw %d Boardid %d\n",nw,boardId);

/*  for (ii=0;ii<(nSamples/16); ii++) {
        for (jj=3;jj>=0;jj--)
        {
          unsigned char samp=((rValue[jj+(ii*4)]&0xff000000)>>24)&0xff;
          mwaveform[xco++]=samp-127;
          //if ((boardId==1)&&(channel==1)&&(xco<10)) printf("%d %d\n",xco-1,samp);
          samp=(rValue[jj+(ii*4)]&0xff0000)>>16;
          mwaveform[xco++]=samp-127;
          //if ((boardId==1)&&(channel==1)&&(xco<10)) printf("%d %d\n",xco-1,samp);
          samp=(rValue[jj+(ii*4)]&0xff00)>>8;
          mwaveform[xco++]=samp-127;
          //if ((boardId==1)&&(channel==1)&&(xco<10)) printf("%d %d\n",xco-1,samp);
          samp=(rValue[jj+(ii*4)]&0xff);
          mwaveform[xco++]=samp-127;
          //if ((boardId==1)&&(channel==1)&&(xco<10)) printf("%d %d\n",xco-1,samp);
        }

  }

*/
 CAENComm_Read32(bdhandle, A_Temperature,(uint32_t*) &buf[nw*4]);
 nw+=1;
 return nw*4;

}


// Writes to a register on the FPGA.
// Input (param): int reg - register number
// Input (param): int n - number of values to write
// Input (param): int* buffer - pointer to the memory to read from.
// Output (return): int - outcome of the operation. See the Acqiris manual for information.
int
A2795Board::write(int reg, int value)
{
      CAENComm_Write32(bdhandle, reg, value);
//    return Acqrs_logicDeviceIO(boardId, "Block1Dev1", reg, n, buffer, 1, 0);
 return 0;
}

// Reads from a register on the FPGA.
// Input (param): int reg - register number
// Input (param): int n - number of values to read
// Output (param): int* buffer - pointer to the memory area to write to.
// Output (return): int - outcome of the operation. See the Acqiris manual for information.
int
A2795Board::read(int reg, int *value)
{
   return CAENComm_Read32(bdhandle, reg,(uint32_t *) value);
    // return Acqrs_logicDeviceIO(boardId, "Block1Dev1", reg, n, buffer, 0, 0);
}
#endif

// For testing purposes. If _simulate_ is defined, it should work without the hardware.
#ifdef _simulate_
void A2795Board::configure (BoardConf conf) {

  printf ( "Conf Board %d offs %f thre %f \n",boardNbr,conf.coff[boardNbr*2],
    conf.cthre[boardNbr*2]);
  printf ( "              offs %f thre %f \n",conf.coff[(boardNbr*2)+1],
    conf.cthre[(boardNbr*2)+1]);

}

void
A2795Board::configureTrig (TrigConf conf)
{

      nSamples=conf.nsamples;
      preSamples = conf.presamples;

      printf("Set nSamples to %d \n",conf.nsamples);
      printf("Set preSamples to %d \n",conf.presamples);
}

void A2795Board::startDPU () {}

int 
A2795Board::isDataRdy()
{
#ifdef _dbg_
    printf("A2795Board::isDataRdy(): wait a second...\n");
    sleep(1);
#endif
    return true;
}

int
A2795Board::ArmTrigger()
{
  return 0;
}
int
A2795Board::fillHeader(DataTile* buf)
{
   //buf->Header.timeinfo= htonl((int) rValue[0]);
   buf->Header.chID= htonl(serialNbr);
}

int
A2795Board::getData(int boa,char* buf)
{
#ifdef _dbg_
    printf("A2795Board::getData(): filling with %d (%c) for %d bytes\n", id, (char)(id+'0'), nSamples);
#endif
    for (int i = 0; i < nSamples; i++)
        buf[i] = (char)(boardNbr+'0');
  return nSamples;
}

#endif
