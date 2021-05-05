#include "Cicathr.h"
#include "DataQueue.h"

class DataReader : public icathr {
    private:
        DataQueue* queue;
        PhysCrate* crate;
        
    public:
        DataReader(PhysCrate cr, DataQueue q);
        ~DataReader();
        void DoTask(); 
};
