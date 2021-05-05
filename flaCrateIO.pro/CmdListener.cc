#include "CmdListener.h"
#include "flaCrate.h"
#include "msgs.h"

CmdListener::CmdListener (flaCrate* owner, Ctcp* conn0) : tcpcom(conn0), ownerCr(owner)
{
    alive = true;
    this->start();
}

CmdListener::~CmdListener () {}

void
CmdListener::parseCommand (void)
{
    short cmdlist[4], ncmd;
    int theco= this->getnextcommand(combuf, cmdlist, &ncmd);
    
	if (theco==MSG_QUIT) alive = false;
	else if (theco == MSG_INIT) 
        {
            printf("CmdListener::parseCommand(): Received INIT message.\n");
            printf("CmdListener::parseCommand(): Setting init parameters...\n");     
            initpar test;
            memcpy(&test, combuf, sizeof(initpar));
            ownerCr->setInitParams(test);

            printf("CmdListener::parseCommand(): Spawning Writer Manager...    ");
		    ownerCr->spawnWrManager();
		    printf("CmdListener::parseCommand(): Writer Manager Spawned.\n");
        }
	else if (theco == MSG_DEDP) 
        {
            printf("CmdListener::parseCommand(): Received DEDP message.\n");
            dedpar *mpar=(dedpar *)combuf;
            
            BoardConf config;
            config.sampInterval = 1e-9;
            config.delayTime = 0.0;
            config.nbrSegments = 1;
            config.coupling = 3;
            config.bandwidth = 0;
            //config.fullScale = 0.050;
            //config.offset = .003;
            config.fullScale = htons(mpar->fullscale)*0.001;
            //config.offset = htons(mpar->thre) * 0.001;;
            short mval = htons(mpar->thre) ;
            config.thresh = mval;
            mval = htons(mpar->offset) ;
            config.offset = mval * config.fullScale/256;
            config.offsetadc = mval ;

            for (int ii=0;ii<16;ii++)
            {
               mval=htons(mpar->coff[ii]);
              //config.coff[ii]=mval * 0.001;
              config.coff[ii]=mval * config.fullScale/256;
              mval=htons(mpar->cthre[ii]); 
              //config.cthre[ii]=mval * 0.001;
              config.cthre[ii]=mval * config.fullScale/256;
               printf("chan %d offs %f thre %f \n",ii,config.coff[ii],config.cthre[ii]);
            }
            
            printf("Fullscale %f Offset %f Thresh %d \n",config.fullScale,config.offset,config.thresh);

            ownerCr->configCrate(config);
	    ownerCr->start();  
        }
	else if (theco == MSG_TRGP)
        {
            printf("CmdListener::parseCommand(): Received TRGP message.\n");
            TrigConf config;
            
            trigpar *mpar=(trigpar *)combuf;

            config.trigClass = 0; // 0: Edge trigger
            config.sourcePattern = 0x00000002; // 0x00000001: channel 1, 0x00000002: channel 2
            config.trigCoupling = 0;
            
            config.channel = 2;
            config.trigSlope = 0; // 0: positive, 1: negative
            config.trigLevel1 = -20.0; // In % of vertical full scale or mV if using an external trigger source.
            config.trigLevel2 = 0.0;
	    printf("Trigpar mode %d \n", mpar->mode); 
	    printf("Trigpar trigmode %d \n", mpar->trigmode); 
            config.nsamples = htons(mpar->mode)*1000 ;
            config.presamples = htons(mpar->trigmode)*1000 ;
            
            ownerCr->configTrigger(config);
	    //ownerCr->start();
        }
	else if (theco == MSG_MODE)
        {
#ifdef DEBUG
		    printf("CmdListener::parseCommand(): Received MODE message.\n");
#endif
        //    ownerCr->ArmTrigger();
        }
	else if (theco == MSG_STRT)
        {
	    ownerCr->start();  
            printf("CmdListener::parseCommand(): Received STRT message.\n");
            
        }
	else if (theco == MSG_FLSH)
        {
            printf("CmdListener::parseCommand(): Received FLSH message.\n");
        }
	else if (theco == MSG_STOP)
        {
            printf("CmdListener::parseCommand(): Received STOP message.\n");
        }
    return;
} // parseCommand

void
CmdListener::DoTask (void)
{
    while (alive)
    {
        parseCommand();
    }
    return;
}
