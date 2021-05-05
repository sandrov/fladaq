#include "A2795Board.h"
#include "structures.h"
#include "veto.h"

#include "CAENComm.h"

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

  int ped[10];
  A2795Board * mA2795[20];

  for (int bus=0; bus<2;bus++)
   for (int nbr=0;nbr<4;nbr++)
   {
     //if ((bus==2)&&(nbr==3)) {}
     //else {
        mA2795[bus*4+nbr]=new A2795Board(nbr,bus);
        printf("instantiate %d\n",mA2795[bus*4+nbr]->boardId);
     //}
  }


  for (int bus=0; bus<2;bus++)
   for (int nbr=0;nbr<4;nbr++)
   {
    if ( mA2795[bus*4+nbr]->boardId!=-1){
      mA2795[bus*4+nbr]->writePedestals(ped);
      //mA2795[bus*4+nbr]->readPedestals(ped);
      //printf("Pedestals %d %d \n",bus,nbr);
      }
   }
 

  for (;;)
  {
  for (int bus=0; bus<2;bus++)
   for (int nbr=0;nbr<4;nbr++)
   {
    if ( mA2795[bus*4+nbr]->boardId!=-1){
      mA2795[bus*4+nbr]->writePedestals(ped);
      mA2795[bus*4+nbr]->readPedestals(ped);
      printf("Pedestals %d %d \n",bus,nbr);
      }
    }
      printf("\n");
    sleep(5); 
    }
}
