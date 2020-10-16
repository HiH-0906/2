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
	// ホストへ接続に行く 0以上なら接続確立
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	if (netHandle_ >= 0)
	{
		active_ = ACTIVE_STATE::INIT;
		std::cout << "初期化したい" << std::endl;
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
