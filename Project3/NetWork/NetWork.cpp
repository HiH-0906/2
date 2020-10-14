#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::s_Instance(new NetWork);



bool NetWork::SetNetWorkMode(NetWorkMode mode)
{
	switch (mode)
	{
	case NetWorkMode::OFFLINE:
		state_ = std::make_unique<NetWorkState>();
		break;
	case NetWorkMode::HOST:
		state_ = std::make_unique<HostState>();
		break;
	case NetWorkMode::GEST:
		state_ = std::make_unique<GestState>();
		break;
	case NetWorkMode::MAX:
		TRACE("NetWorkState¶¬Žž ˆÙíŒŸo\n");
		return false;
		break;
	default:
		TRACE("NetWorkState¶¬Žž ˆÙíŒŸo\n");
		return false;
		break;
	}
	return false;
}

bool NetWork::GetActive(void)
{
	return state_->GetActive();
}

IPDATA NetWork::GetIP(void)
{
	IPDATA myIP;
	GetMyIPAddress(&myIP);
	return myIP;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{

	return false;
}

NetWorkMode NetWork::GetMode(void)
{
	if (state_ == nullptr)
	{
		return NetWorkMode::OFFLINE;
	}
	return state_->GetMode();
}

NetWork::NetWork()
{
	state_ = nullptr;
}

NetWork::~NetWork()
{
}
