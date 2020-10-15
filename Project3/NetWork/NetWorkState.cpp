#include "NetWorkState.h"
#include "../_debug/_DebugConOut.h"

NetWorkState::NetWorkState()
{
	active_ = false;
	netHandle_ = -1;
}

bool NetWorkState::Update(void)
{
	return CheckNetState();
}

NetWorkState::~NetWorkState()
{
}

void NetWorkState::SendMes(POS_DATA data)
{
}

void NetWorkState::RecvMes(Vector2& pos)
{
	
}

Vector2 RecvMes(void)
{
	return Vector2{};
}

bool NetWorkState::GetActive(void)
{
	return active_;
}

bool NetWorkState::ConnectHost(IPDATA hostIP)
{
	return false;
}

void NetWorkState::CloseNetWork(void)
{
	DxLib::CloseNetWork(netHandle_);
}
