#include "DxLib.h"
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// 接続受付開始していたら0なのでこんな感じ
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;
}

HostState::~HostState()
{
}

void HostState::RecvMes(Vector2& pos)
{
	if (GetNetWorkDataLength(netHandle_) >= sizeof(POS_DATA))
	{
		POS_DATA data;
		NetWorkRecv(netHandle_, &data, sizeof(POS_DATA));
		pos = { data.x,data.y };
	}
}

bool HostState::CheckNetState(void)
{
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		// 今回は複数人を想定していないので一人接続してきたら新規受付を終了する
		netHandle_ = tmpID;
		StopListenNetWork();
	}

	if (GetLostNetWork()!=-1)
	{
		// ゲストから切られた場合再接続待ち
		PreparationListenNetWork(portNum_);
		// ホストから辞めたいときは手動でCloseNetWork呼ぼうネ
		return false;
	}
	return true;
}
