#include "AcqBoard.h"
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



AcqBoard::AcqBoard(int nbr, ViSession id) : boardNbr(nbr), boardId(id) 
{
#ifndef _simulate_
 Acqrs_getInstrumentData(boardId, boardname, &serialNbr, &busNbr, &slotNbr);
#else
 serialNbr=boardNbr;
#endif
 printf ("BoardId %x serial %d bus %d slot %d\n",boardId,serialNbr, busNbr, slotNbr);
}

AcqBoard::~AcqBoard(){}

#ifndef _simulate_

// Configures the board.
// Input (param): BoardConf conf - structure with the configuration parameters.
void
AcqBoard::configure (BoardConf conf)
{		

  ViStatus status;


        printf( "clearing FPGA \n");
        status = Acqrs_configLogicDevice(boardId, "Block1DevAll", "", 1); // Loads the FPGA firmware

        status = Acqrs_configLogicDevice(boardId, "Block1Dev1","warp2_0.bit", 0); // Loads the FPGA firmware
        AcqUtil::checkStatus(status, "Firmware loading successful.");


       //status = Acqrs_calibrate(boardId);
       //AcqUtils::checkStatus(status, "Calibrated.");

        status = AcqrsD1_configMode(boardId, 1, 0, 0);
        //sourcePattern = 0x80000000;



    // Configure timebase
	AcqrsD1_configHorizontal(boardId, conf.sampInterval, conf.delayTime);
	AcqrsD1_configMemory(boardId, nSamples, conf.nbrSegments);


 printf ( "Conf Board %d offs %f thre %f \n",boardNbr,conf.coff[boardNbr*2],
    conf.cthre[boardNbr*2]);
  printf ( "              offs %f thre %f \n",conf.coff[(boardNbr*2)+1],
    conf.cthre[(boardNbr*2)+1]);
 
        printf("off %f thre %f \n", conf.coff[(boardNbr)*2],conf.cthre[(boardNbr)*2]);
        printf("off %f thre %f\n", conf.coff[(boardNbr)*2+1],conf.cthre[(boardNbr)*2+1]);
	// Configure vertical settings of channels
	//AcqrsD1_configVertical(boardId, 1, conf.fullScale, conf.offset+conf.coff[(boardNbr)*2]+conf.cthre[(boardNbr)*2], conf.coupling, conf.bandwidth);
	//AcqrsD1_configVertical(boardId, 2, conf.fullScale, conf.offset+conf.coff[(boardNbr)*2+1]+conf.cthre[(boardNbr)*2+1], conf.coupling, conf.bandwidth);
	AcqrsD1_configVertical(boardId, 1, conf.fullScale, conf.offset+conf.coff[(boardNbr)*2], conf.coupling, conf.bandwidth);
	AcqrsD1_configVertical(boardId, 2, conf.fullScale, conf.offset+conf.coff[(boardNbr)*2+1], conf.coupling, conf.bandwidth);
	mThre=128+conf.offsetadc-conf.thresh;
        printf("channel thresh 1 %d thresh2 %d \n");
	printf("AcqBoard::configure(): board %d configured.\n", boardNbr);
}

// Configures the trigger parameters.
// Input (param): TrigConf conf - structure with the trigger parameters.
void
AcqBoard::configureTrig (TrigConf conf)
{

      nSamples=conf.nsamples;
      preSamples = conf.presamples;

      printf("Set nSamples to %d \n",conf.nsamples);
      printf("Set preSamples to %d \n",conf.presamples);
	//AcqrsD1_configTrigClass(boardId, conf.trigClass, conf.sourcePattern, 0, 0, 0.0, 0.0);
	//AcqrsD1_configTrigSource(boardId, conf.channel, conf.trigCoupling, conf.trigSlope, conf.trigLevel1, conf.trigLevel2);
  
        ViInt32 regValue;
        regValue=nSamples/16;
        Acqrs_logicDeviceIO(boardId, "Block1Dev1", 71, 1, &regValue, WRITE, 0);

        regValue=24000/16; //pretrigger samples
        Acqrs_logicDeviceIO(boardId, "Block1Dev1", 72, 1, &regValue, WRITE, 0);
}

// Starts the DPU on the board.
void
AcqBoard::startDPU ()
{
  ViInt32 regValue;

    AcqUtil::checkStatus(AcqrsD1_acquire(boardId));
    printf("AcqBoard::startDPU(): core of board (%d,%ld) started.\n", boardNbr, (long)boardId);

    regValue=0x80FF0100;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 3, 1, &regValue, WRITE, 0);

    regValue=0x100;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 64, 1, &regValue, WRITE, 0);

    //Reset DDR
    regValue=0x70;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 44, 1, &regValue, WRITE, 0);
    regValue=0x70;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 52, 1, &regValue, WRITE, 0);

    regValue=0x00;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 44, 1, &regValue, WRITE, 0);
    regValue=0x00;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 52, 1, &regValue, WRITE, 0);

    regValue=0x80010000;
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 8, 1, &regValue, WRITE, 0);

    //regValue=0x804;
    regValue=0x4 + (mThre*16);
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 68, 1, &regValue, WRITE, 0);
    //regValue=0x800;
    regValue=0x0 + (mThre*16);
    Acqrs_logicDeviceIO(boardId, "Block1Dev1", 68, 1, &regValue, WRITE, 0);

}

// Tells if data are ready
// Output (return): true if there are ready data, false otherwise.
int
AcqBoard::isDataRdy()
{
     ViInt32 regValue;
     ViBoolean done = 0;

     long timeoutCounter = 250000; // Timeout waiting for a trigger
     long errTimeoutCounter = 500000; // Timeout for acquisition completion
     //long errTimeoutCounter = 1; // Timeout for acquisition completion
#ifdef _dbg_
    printf("AcqBoard::isDataRdy(): BoardId %d BoardNbr %d\n",boardId, boardNbr);
#endif


	while (!done && errTimeoutCounter--)
	{
	//	AcqrsD1_acqDone(boardId, &done); // Poll for the end of the acquisition
              Acqrs_logicDeviceIO(boardId, "Block1Dev1", 69, 1, &regValue, READ, 0);
             done=(regValue&0x1);
		if (!timeoutCounter--) // Trigger timeout occured
		{
			printf("AcqBoard::isDataRdy(): Slow trigger...\n");
                       vetoOff();
		//AcqrsD1_forceTrig(boardId); // Force a 'manual' (or 'software') trigger
                }
	}
	return done;
}

int 
AcqBoard::ArmTrigger()
{
  ViInt32 regValue;

#ifdef _dbg_
    printf("AcqBoard::ArmTrigger(): wait a second...\n");
#endif


  regValue=0x1 + (mThre*16);
  //regValue=0x801;
  //regValue=0x809;  //added negative discrim
  Acqrs_logicDeviceIO(boardId, "Block1Dev1", 68, 1, &regValue, WRITE, 0);

  return 0;
}

int
AcqBoard::fillHeader(DataTile* buf)
{
  ViInt32 rValue[950*1024];
#ifdef _dbg_
    printf("AcqBoard::fillHeader(): wait a second...\n");
#endif

   Acqrs_logicDeviceIO(boardId, "Block1Dev1", 70, 1, rValue, READ, 0);
   //buf->Header.info3=((long)inet_lnaof(mId))&0xff;
   buf->Header.timeinfo= htonl((int) rValue[0]);
   buf->Header.chID= htonl(serialNbr);
}

// Writes the data gathered by the board starting from the passed address.
// Output (param): char* buf - pointer to the memory area to write to.
int
AcqBoard::getData(int channel,char* buf)
{


#ifdef _dbg_
    printf("AcqBoard::getData(): wait a second...\n");
#endif

  // ### Readout the data ###
  ViStatus status;
  int xco,ii,jj,totwords;
  ViInt32 regValue,rValue[950*1024];
  char *mwaveform=(char *)buf;


	// local buffer
//	char* wf = new char[dataArraySize*2];

  //regValue=0x803;
  regValue=0x3 + (mThre*16);
  //regValue=0x80b3; //added negative discrim
  Acqrs_logicDeviceIO(boardId, "Block1Dev1", 68, 1, &regValue, WRITE, 0);

  //Set DDR internal bus access
  regValue=0x100;
  //if (channel==1)
        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 44, 1, &regValue, WRITE, 0);
  //else
  regValue=0x100;
        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 52, 1, &regValue, WRITE, 0);

  //Set buffer buf address
  if (channel==1)
        regValue=0x0;
  else
        regValue=0x1;

        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 2, 1, &regValue, WRITE, 0);
        regValue=0x4000 -(preSamples - 2000);
        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 1, 1, &regValue, WRITE, 0);

  xco=0;

       totwords=nSamples/4;
       status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 0, totwords, rValue, READ, 0);

//for (int ii=0;ii<10;ii++)
//printf("%d ",rValue[ii]);
// printf("\n");
// printf("start decoding Boardid %d\n",boardId);

  for (ii=0;ii<(nSamples/16); ii++) {
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

 //Set DDR user port access

  regValue=0x0;
  //if (channel==1)
        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 44, 1, &regValue, WRITE, 0);
  //else
  regValue=0x0;
        status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 52, 1, &regValue, WRITE, 0);



	// Read the channel 1 waveform samples
//	status = AcqrsD1_readData(boardId, 1, &readParams, (void *)wf, &wfDesc, &segDesc);
//	AcqUtil::checkStatus(status);
//	printf("AcqBoard::getData(): samples acquired on channel 1: %d \n", (int)wfDesc.returnedSamplesPerSeg);
//	int iFirst = (int)wfDesc.indexFirstPoint;
//	for (int i = 0; i < nSamples; i++)
 //         buf[i] = wf[i+iFirst];

/*
   status = Acqrs_logicDeviceIO(boardId, "Block1Dev1", 70, 1, rValue, READ, 0);
   DataTile *thetile=(DataTile *) buf;
   thetile->Header.timeinfo= (int) rValue[0];
   //thetile->Header.chID= channel+boardId*16+(((long)inet_lnaof(mId))&0xff)*256;
   thetile->Header.chID= channel+boardId*16+HOST*256;
*/
	
/*	
	static bool isfirst = true;
	if(isfirst){
	  FILE* f;
	  f=fopen("wavef.txt","w");
	  fprintf(f,"t/I:wf1/I:wf2/I\n");
	  for (ii = 0; ii<nSamples; ii++){
	    int swf1 = buf[ii];
	    int swf2 = buf[ii+nSamples];
	    //fprintf(f,"%d %hx %hx\n", ii, buf[ii] & 0x000000ff, buf[ii+nSamples] & 0x000000ff);
	    fprintf(f,"%d %d %d\n",ii,swf1,swf2);
	  }
	  fclose(f);
	  isfirst = false;
	}

*/
	return nSamples;

}


// Writes to a register on the FPGA.
// Input (param): long reg - register number
// Input (param): long n - number of values to write
// Input (param): long* buffer - pointer to the memory to read from.
// Output (return): long - outcome of the operation. See the Acqiris manual for information.
long
AcqBoard::write(long reg, long n, long* buffer)
{
    return Acqrs_logicDeviceIO(boardId, "Block1Dev1", reg, n, buffer, 1, 0);
}

// Reads from a register on the FPGA.
// Input (param): long reg - register number
// Input (param): long n - number of values to read
// Output (param): long* buffer - pointer to the memory area to write to.
// Output (return): long - outcome of the operation. See the Acqiris manual for information.
long
AcqBoard::read(long reg, long n, long* buffer)
{
    return Acqrs_logicDeviceIO(boardId, "Block1Dev1", reg, n, buffer, 0, 0);
}
#endif

// For testing purposes. If _simulate_ is defined, it should work without the hardware.
#ifdef _simulate_
void AcqBoard::configure (BoardConf conf) {

  printf ( "Conf Board %d offs %f thre %f \n",boardNbr,conf.coff[boardNbr*2],
    conf.cthre[boardNbr*2]);
  printf ( "              offs %f thre %f \n",conf.coff[(boardNbr*2)+1],
    conf.cthre[(boardNbr*2)+1]);

}

void
AcqBoard::configureTrig (TrigConf conf)
{

      nSamples=conf.nsamples;
      preSamples = conf.presamples;

      printf("Set nSamples to %d \n",conf.nsamples);
      printf("Set preSamples to %d \n",conf.presamples);
}

void AcqBoard::startDPU () {}

int 
AcqBoard::isDataRdy()
{
#ifdef _dbg_
    printf("AcqBoard::isDataRdy(): wait a second...\n");
    sleep(1);
#endif
    return true;
}

int
AcqBoard::ArmTrigger()
{
  return 0;
}
int
AcqBoard::fillHeader(DataTile* buf)
{
   //buf->Header.timeinfo= htonl((int) rValue[0]);
   buf->Header.chID= htonl(serialNbr);
}

int
AcqBoard::getData(int boa,char* buf)
{
#ifdef _dbg_
    printf("AcqBoard::getData(): filling with %d (%c) for %d bytes\n", id, (char)(id+'0'), nSamples);
#endif
    for (int i = 0; i < nSamples; i++)
        buf[i] = (char)(boardNbr+'0');
  return nSamples;
}

#endif
