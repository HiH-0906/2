#include <iostream>
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
	active_ = ACTIVE_STATE::NON;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	auto handle = ConnectNetWork(hostIP, portNum_);
	// ホストへ接続に行く 0以上なら接続確立
	if (handle >= 0)
	{
		netHandleList_.push_back({ handle, 0 });
		active_ = ACTIVE_STATE::INIT;
		TRACE("初期化したい");
		return true;
	}
	return false;
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
