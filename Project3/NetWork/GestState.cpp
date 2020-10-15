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
		active_ = true;
		return true;
	}
	return false;
}

void GestState::SendMes(POS_DATA data)
{
	NetWorkSend(netHandle_, &data, sizeof(POS_DATA));
}

bool GestState::CheckNetState(void)
{
	if (GetLostNetWork() != -1)
	{
		// ホストに切断されたらこっちも閉じる
		CloseNetWork();
		return false;
	}
	return true;
}
