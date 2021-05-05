#ifndef _kcmd_
#define _kcmd_

#include <stdio.h>
#include <string.h>

#define CMD_NONE 0
#define CMD_STOP 10
#define CMD_STATUS 100

class keycmd 
{
  public:
   virtual int GetNextCommand();
};

#endif

