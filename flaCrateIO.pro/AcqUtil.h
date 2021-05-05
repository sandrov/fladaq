#ifndef _AcqUtil_
#define _AcqUtil_

#include <stdio.h>
#include "AcqirisImport.h"

namespace AcqUtil
{
    void checkStatus(ViStatus status);
    void checkStatus(ViStatus status, char* msg);
    
}

#endif
