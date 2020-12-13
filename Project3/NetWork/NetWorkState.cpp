#include "NetWorkState.h"
#include "../_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = ACTIVE_STATE::OFFLINE;
	countDownRoomTime_ = {};
	countDownGameTime_ = {};
	countDown_ = false;
	gameStart_ = false;
	playerID_ = -1;
	playerMax_ = -1;
}

bool NetWorkState::Update(void)
{
	return CheckNetState();
}

NetWorkState::~NetWorkState()
{
}

Vector2 RecvMes(void)
{
	return Vector2{};
}

ACTIVE_STATE NetWorkState::GetActive(void)
{
	return active_;
}

const HandleList& NetWorkState::GetNetHandle(void)const
{
	return netHandleList_;
}

bool NetWorkState::GetCountStart(void)
{
	return countDown_;
}

bool NetWorkState::GetGameStart(void)
{
	return gameStart_;
}

const std::chrono::system_clock::time_point& NetWorkState::GetCountDownRoomTime(void)
{
	std::lock_guard<std::mutex> lock(downMtx_);
	return countDownRoomTime_;
}

const std::chrono::system_clock::time_point& NetWorkState::GetCountDownGameTime(void)
{
	std::lock_guard<std::mutex> lock(gameMtx_);
	return countDownGameTime_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}

bool NetWorkState::SetActive(ACTIVE_STATE state)
{
	active_ = state;
	return true;
}

const int& NetWorkState::GetID(void) const
{
	return playerID_;
}

const int NetWorkState::GetMax(void) const
{
	if (playerMax_ == -1)
	{
		return INT_MAX;
	}
	return playerMax_;
}

void NetWorkState::SetGameStart(bool flag)
{
	gameStart_ = flag;
}

void NetWorkState::CloseNetWork(void)
{
	DxLib::CloseNetWork(netHandleList_.front().handle);
}

void NetWorkState::SetCountDownRoomTime(std::chrono::system_clock::time_point time)
{
	std::lock_guard<std::mutex> lock(downMtx_);
	countDownRoomTime_ = time;
	countDown_ = true;
}

void NetWorkState::SetCountDownGameTime(std::chrono::system_clock::time_point time)
{
	std::lock_guard<std::mutex> lock(gameMtx_);
	countDownGameTime_ = time;
}

void NetWorkState::SetPlayerID(int id, unsigned int max)
{
	playerID_ = id;
	playerMax_ = max;
}
