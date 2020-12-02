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
	startTime_ = {};
	playerMax_ = 0;
	std::cout << "接続待機" << std::endl;
}

HostState::~HostState()
{
}

bool HostState::CheckNetState(void)
{
	auto handle = GetNewAcceptNetWork();
	if (handle != -1)
	{
		if (netHandleList_.size() == 0)
		{
			startTime_ = std::chrono::system_clock::now();
		}
		netHandleList_.push_back({ handle, 0 });
		sendData data[2];
		unionTimeData time = { startTime_ };
		countTime_ = startTime_;
		countDown_ = true;
		data[0].idata = time.idata[0];
		data[1].idata = time.idata[1];
		lpNetWork.SendMes(MES_TYPE::COUNT_DOWN_ROOM, MesDataList{ data[0],data[1] }, handle);
		playerMax_++;
		std::cout << "ゲストの接続" << std::endl;
	}
	if (netHandleList_.size() != 0)
	{
		auto timeCnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime_).count();
		if (timeCnt >= INIT_COUNT_TIME)
		{
			if (playerMax_ == lpNetWork.GetRevCount())
			{
				active_ = ACTIVE_STATE::STANBY;
			}
			else if(active_ != ACTIVE_STATE::INIT)
			{
				sendData data[2];
				data[0].idata = 5;
				data[1].idata = playerMax_;
				for (auto handle = netHandleList_.begin(); handle != netHandleList_.end(); handle++)
				{
					lpNetWork.SendMes(MES_TYPE::ID, MesDataList{ data[0],data[1] }, handle->first);
					data[0].idata += 5;
				}
				active_ = ACTIVE_STATE::INIT;
			}
			else
			{

			}
		}
	}
	return true;
}
