#include "NetWorkState.h"
#include "../_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = ACTIVE_STATE::NON;
	netHandle_ = -1;
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

int NetWorkState::GetNetHandle(void)
{
	return netHandle_;
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

void NetWorkState::CloseNetWork(void)
{
	DxLib::CloseNetWork(netHandle_);
}
