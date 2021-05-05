#include "WarpCrate.h"

int main (void)
{
    WarpCrate* wcr = new WarpCrate();
    wcr->getEvmConnection();
    wcr->spawnCmdListener();
    wcr->waitStopCmd();
    return 0;
}
