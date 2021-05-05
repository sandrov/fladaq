
WARPID = $(shell uname -i)

#SQLLIBS       = -L/usr/lib/mysql -lmysqlclient 
SQLLIBS       = -L/usr/lib/ -lmysqlclient 
ifeq ($(WARPID),x86_64)
#SQLLIBS       = -L/usr/lib64/mysql -lmysqlclient 
SQLLIBS       = -lmysqlclient 
endif

MYSQLFLAGS    = $(shell mysql_config --cflags)

LDFLAGS = -lpthread  -lz $(SQLLIBS)
CCFLAGS =  $(MYSQLFLAGS)
CC = g++ -D DEBUG
LD = g++

.cc.o  :
	@ $(RM) $@
	$(CC) $(CCFLAGS) -g -c $<



Ctcpux.o : Ctcpux.h

mpicaobjs = mpica.o Cicaevm.o Cicadb.o Cicawrihdl.o Cicacrate.o Ctcpux.o \
	    Cudpux.o Cicathr.o Cicatrg.o Cicaconfdb.o Cicalogdb.o Cicaevrecv.o Ckeycmd.o

rbrowobjs = resbrowser.o Cresoitem.o Cudpux.o Ctcpux.o

mpicawriobjs = mpicawri.o Cicadb.o Cicaevmhdl.o Cicawri.o Cicacrate.o \
               Cicathr.o Cicaevrecv.o \
               Cicarunf.o Cicadskfil.o Cicalogdb.o \
               Cicaann.o Cicaactimon.o \
               Ctcpux.o Cudpux.o Cicaconfdb.o
testbroobjs = testrbrow.o Cresbrow.o Cresoitem.o Cudpux.o Ctcpux.o \
               Cicathr.o Cicapertsk.o Cicadb.o Cicalogdb.o

mpica : ${mpicaobjs} 
	$(LD) -o $@ ${mpicaobjs} -lmysqlclient $(LDFLAGS) 

mpicawri : ${mpicawriobjs} 
	$(LD) $(LDFLAGS) -o $@ ${mpicawriobjs} -lmysqlclient -lpthread

rbrow : ${rbrowobjs} 
	$(LD) $(LDFLAGS) -o $@ ${rbrowobjs} 

testbro: ${testbroobjs}
	$(LD) $(LDFLAGS) -o $@ ${testbroobjs} -lmysqlclient -lz
