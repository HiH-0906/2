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
