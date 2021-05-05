#include <iostream>

class comma
{
     public:
         comma();
         virtual ~comma();

     virtual int getnextcommand(char *combuf){ return 0; };
};
