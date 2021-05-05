#include "flaCrate.h"

int main (void)
{
    flaCrate* fcr = new flaCrate();
    fcr->getEvmConnection();
    fcr->spawnCmdListener();
    fcr->waitStopCmd();
    return 0;
}
