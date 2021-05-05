#include "DataManager.h"

DataManager::DataManager (PhysCrate* cr) : pCrate(cr) {}

DataManager::~DataManager (void) {}

void
DataManager::waitData()
{
    pCrate->waitData();
}

void
DataManager::ArmTrigger()
{
    pCrate->ArmTrigger();
}

bool
DataManager::dataAvail()
{
    return pCrate->dataAvail();
}

DataTile*
DataManager::getData (void)
{
#ifdef _dbg_
    printf("DataManager::getData()...\n");
#endif

    //pCrate->waitData();

#ifdef _dbg_
    printf("DataManager::getData(): data ready.\n");
#endif

    DataTile* tile = pCrate->getData();

#ifdef _dbg_
    printf("DataManager::getData(): got data.\n");
#endif

    memcpy(&(tile->Header.token), "DATA", 4);
    //tile->Header.info3=ntohl(pCrate->getId());
    return tile;
}

statpack *
DataManager::getStat (void)
{
    statpack* pack = new statpack;
    memcpy(&(pack->token), "STAT", 4);
    //pack->crateid = htonl(158);
    pack->crateid = htonl(pCrate->getId());
    pack->memstat1 = 0;
    pack->memstat2 = 0;
    pack->size = htonl(28);
    return pack;
}
