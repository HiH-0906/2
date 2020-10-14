#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
	active_ = false;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	// ホストへ接続に行く 0以上なら接続確立
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	if (netHandle_ >= 0)
	{
		return true;
	}
	return false;
}
