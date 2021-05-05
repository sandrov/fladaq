#include "DataQueue.h"

DataQueue::DataQueue() : first(NULL), last(NULL) {};

DataQueue::~DataQueue()
{
    pthread_mutex_destroy (mtx);
}

void
DataQueue::enqueue(void* ptr)
{
    pthread_mutex_lock(mtx);
    
    Node n = new Node();
    n.prev = NULL;
    n.next = first;
    first.prev = n;
    first = n;
    if (last == NULL)
        last = n;
        
    pthread_mutex_unlock(mtx);
}

void*
DataQueue::dequeue()
{
    pthread_mutex_lock(mtx);
    void* ptr;
    
    if (last == NULL)
        return NULL;
        
    ptr = first.elem;
    if (first == last)
    {

        delete first;
        first = NULL;
        last = NULL;
        return ptr;
    }
    last.next = NULL;
    
        
    
    pthread_mutex_unlock(mtx);
}
