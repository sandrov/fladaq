#include "Ctcpux.h"

int main () 
{
    Ctcp* c1 = new Ctcp();
    Ctcp* c2 = new Ctcp();
    
    c1.Connect(localhost, 4000);
    c2.Connect(localhost, 4001);



    return 0;
}
