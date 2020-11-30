#include "NetWorkState.h"
#include "../_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = ACTIVE_STATE::OFFLINE;
	countTime_ = {};
	countStart_ = false;
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
	return countStart_;
}

const std::chrono::system_clock::time_point& NetWorkState::GetCountDownTime(void) const
{
	return countTime_;
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

const int& NetWorkState::GetMax(void) const
{
	return playerMax_;
}

void NetWorkState::CloseNetWork(void)
{
	DxLib::CloseNetWork(netHandleList_.front().first);
}

void NetWorkState::SetCountTime(std::chrono::system_clock::time_point time)
{
	if (countStart_)
	{
		return;
	}
	countTime_ = time;
	countStart_ = true;
}

void NetWorkState::SetPlayerID(int id, unsigned int max)
{
	playerID_ = id;
	playerMax_ = max;
}
