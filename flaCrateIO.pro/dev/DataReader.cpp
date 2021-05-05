#include "DataReader.h"

DataReader::DataReader(DataQueue q) : queue(q), crate(cr)
{
    this->start();
} // DataReader()

DataReader::~DataReader() {}

DataReader::DoTask()
{
    while (true) {
        crate.waitData();
        queue.enqueue(crate.getData());
    } // while   
}
