CCFLAGS= -Wall -O2 -fPIC -DLINUX -D_LINUX -I./include
LDFLAGS = -lpthread -lCAENComm
#LDFLAGS = -L/usr/lib/ -lpthread -lCAENComm
#LDFLAGS = -lpthread 
#CC = g++ -D _dbg_ 
#CC = g++  -D _simulate_
CC = g++  
LD = g++

.cc.o  :
	@ $(RM) $@
	$(CC) $(CCFLAGS) -g -c $<
	
.cpp.o  :
	@ $(RM) $@
	$(CC) $(CCFLAGS) -g -c $<

Ctcpux.o : Ctcpux.h

warpobjs = mWarp.o WarpCrate.o ConnectionHandler.o Ctcpux.o CmdListener.o Cicathr.o Ctcpcom.o Ccomma.o \
           DataManager.o WrHandler.o WrManager.o PhysCrate.o AcqBoard.o AcqUtil.o Cudpux.o veto.o 


setpedobjs = setped.o flaCrate.o ConnectionHandler.o Ctcpux.o CmdListener.o Cicathr.o Ctcpcom.o Ccomma.o \
           DataManager.o WrHandler.o WrManager.o PhysCrate.o A2795Board.o Cudpux.o veto.o 

A2795objs = mfla.o flaCrate.o ConnectionHandler.o Ctcpux.o CmdListener.o Cicathr.o Ctcpcom.o Ccomma.o \
           DataManager.o WrHandler.o WrManager.o PhysCrate.o A2795Board.o Cudpux.o veto.o 

mfla : ${A2795objs} 
	$(LD) -o $@ ${A2795objs} $(LDFLAGS) 

mWarp : ${warpobjs} 
	$(LD) $(LDFLAGS) -o $@ ${warpobjs}

setped : ${setpedobjs}
	$(LD) $(LDFLAGS) -o $@ ${setpedobjs}
