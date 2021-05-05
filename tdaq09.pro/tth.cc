#include "Cicathr.h"
#include <stdio.h>

class newth : public icathr
{
  public:
//   newth(){};
//   ~newth(){};
   void DoTask(){printf("I'm a new class thread\n");};
};

main()
{
  newth myth;

  printf("Hello I'm the main\n");
  myth.start();
  myth.join();
  printf("Hello thread\n");
}

