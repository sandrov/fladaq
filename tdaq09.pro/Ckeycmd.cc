#include "Ckeycmd.h"

int
keycmd::GetNextCommand()
{
  char cmdstr[256];

  fgets(cmdstr,256,stdin);

  if ((strncmp(cmdstr,"stat",4)==0) ||
      (strncmp(cmdstr,"STAT",4)==0))
   {
       return CMD_STATUS;
   }
  else if ((strncmp(cmdstr,"sto",3)==0) || 
         (strncmp(cmdstr,"STO",3)==0))
  {
       return CMD_STOP;
  }
  return CMD_NONE;
}
