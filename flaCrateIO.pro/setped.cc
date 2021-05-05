#include "A2795Board.h"
#include "structures.h"
#include "veto.h"

#include "CAENComm.h"
#include "A2795.h"

#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define WRITE 1
#define READ 0

#define HOST 1

int 
main(void) {

  for (int bus=0; bus<2;bus++)
   for (int nbr=0;nbr<4;nbr++)
   {
    A2795Board mA2795(nbr,bus);
  }
}
