#include <iostream>
#include <DxLib.h>
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// 接続受付開始していたら0なのでこんな感じ
	PreparationListenNetWork(portNum_);
	active_ = ACTIVE_STATE::WAIT;
	std::cout << "接続待機" << std::endl;
}

HostState::~HostState()
{
}

bool HostState::CheckNetState(void)
{
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		// 今回は複数人を想定していないので一人接続してきたら新規受付を終了する
		netHandle_ = tmpID;
		active_ = ACTIVE_STATE::INIT;
		std::cout << "接続されました" << std::endl;
		StopListenNetWork();
	}

	if (GetLostNetWork()!=-1)
	{
		// ゲストから切られた場合再接続待ち
		PreparationListenNetWork(portNum_);
		active_ = ACTIVE_STATE::WAIT;
		std::cout << "切断されました" << std::endl;
		// ホストから辞めたいときは手動でCloseNetWork呼ぼうネ
		return false;
	}
	return true;
}
