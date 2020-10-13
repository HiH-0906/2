#include "NetWork.h"

IPDATA NetWork::GetIP(void)
{
	IPDATA myIP;
	GetMyIPAddress(&myIP);
	return myIP;
}

NetWork::~NetWork()
{
}
