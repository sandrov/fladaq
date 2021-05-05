#include <pthread.h>

typedef struct
{
    Node* next;
    Node* prev;
    void* elem;
} Node;

class DataQueue
{
    private:
        pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
        Node* first;
        Node* last;
       
    public: 
        DataQueue();
        ~DataQueue();
        void enqueue(void *);
        void* dequeue();
};
