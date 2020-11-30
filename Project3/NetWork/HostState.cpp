#include <iostream>
#include <DxLib.h>
#include "HostState.h"
#include "NetWork.h"
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
	//auto tmpID = GetNewAcceptNetWork();
	//if (tmpID != -1)
	//{
	//	// 今回は複数人を想定していないので一人接続してきたら新規受付を終了する
	//	netHandleList_ = tmpID;
	//	active_ = ACTIVE_STATE::INIT;
	//	std::cout << "接続されました" << std::endl;
	//	StopListenNetWork();
	//}

	//if (GetLostNetWork() != -1)
	//{
	//	// ゲストから切られた場合再接続待ち
	//	PreparationListenNetWork(portNum_);
	//	active_ = ACTIVE_STATE::WAIT;
	//	netHandleList_ = -1;
	//	std::cout << "切断されました" << std::endl;
	//	auto ipVec = lpNetWork.GetIP();
	//	for (auto ip : ipVec)
	//	{
	//		if (ip.d1 != 0)
	//		{
	//			std::string mes = ip.d1 == 192 ? "グローバル" : "ローカル";
	//			TRACE("自分の%sIPアドレスは%d.%d.%d.%dです\n", mes.c_str(), ip.d1, ip.d2, ip.d3, ip.d4);
	//		}
	//	}
	//	// ホストから辞めたいときは手動でCloseNetWork呼ぼうネ
	//	return false;
	//}
	return true;
}
