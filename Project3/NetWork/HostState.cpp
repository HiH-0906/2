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
	TRACE("接続待機");
}

HostState::~HostState()
{
}

bool HostState::CheckNetState(void)
{
	auto handle = GetNewAcceptNetWork();
	auto lost = GetLostNetWork();
	if (handle != -1)
	{
		if (netHandleList_.size() == 0)
		{
			startTime_ = std::chrono::system_clock::now();
		}
		netHandleList_.push_back({ handle, 0,0 });
		sendData data[2];
		unionTimeData time = { startTime_ };
		countDownRoomTime_ = startTime_;
		countDown_ = true;
		playerID_ = 0;
		data[0].idata = time.idata[0];
		data[1].idata = time.idata[1];
		lpNetWork.SendMes(MES_TYPE::COUNT_DOWN_ROOM, MesDataList{ data[0],data[1] }, handle);
		playerMax_++;
		TRACE("ゲストの接続");
	}
	if (lost != -1)
	{
		for (auto handle = netHandleList_.begin(); handle != netHandleList_.end(); handle++)
		{
			if (handle->handle != lost)
			{
				continue;
			}
			TRACE("GUESTの切断：%d\n", handle->id);
			playerMax_--;
			handle->state = -1;
		}
	}
	if (netHandleList_.size() != 0)
	{
		auto timeCnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime_).count();
		if (timeCnt >= INIT_COUNT_TIME)
		{
			StopListenNetWork();
			
			if (active_ != ACTIVE_STATE::INIT)
			{
				sendData data[2];
				data[0].idata = 5;
				data[1].idata = playerMax_ + 1;
				for (auto handle = netHandleList_.begin(); handle != netHandleList_.end(); handle++)
				{
					if (handle->state == -1)
					{
						continue;
					}
					lpNetWork.SendMes(MES_TYPE::ID, MesDataList{ data[0],data[1] }, handle->handle);
					handle->id = data[0].idata;
					data[0].idata += 5;
				}
				active_ = ACTIVE_STATE::INIT;
			}
			
		}
	}
	return true;
}
