#include "NetWork.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::s_instance(new NetWork);

IPDATA NetWork::GetIP(void)
{
	IPDATA myIP;
	GetMyIPAddress(&myIP);
	return myIP;
}

NetWork::NetWork()
{
}

NetWork::~NetWork()
{
}
