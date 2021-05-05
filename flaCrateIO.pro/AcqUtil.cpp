#include "AcqUtil.h"

void
AcqUtil::checkStatus (ViStatus status, char* msg)
{
	if(status != VI_SUCCESS)
	{
		char message[512];
		Acqrs_errorMessage(VI_NULL, status, message, 512);
		printf("%s\n", message);
	}
	else
	    printf("%s\n", msg);
}

void
AcqUtil::checkStatus (ViStatus status)
{
	if(status != VI_SUCCESS)
	{
		char message[512];
		Acqrs_errorMessage(VI_NULL, status, message, 512);
		printf("%s\n", message);
	}
}
