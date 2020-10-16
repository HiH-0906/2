#include <iostream>
#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::s_Instance(new NetWork);



bool NetWork::UpDate(void)
{
	if (!state_)
	{
		return false;
	}
	return state_->Update();
}

bool NetWork::SetNetWorkMode(NetWorkMode mode)
{
	switch (mode)
	{
	case NetWorkMode::OFFLINE:
		state_ = std::make_unique<NetWorkState>();
		break;
	case NetWorkMode::HOST:
		state_ = std::make_unique<HostState>();
		break;
	case NetWorkMode::GEST:
		state_ = std::make_unique<GestState>();
		break;
	case NetWorkMode::MAX:
		TRACE("NetWorkState生成時 異常検出\n");
		return false;
		break;
	default:
		TRACE("NetWorkState生成時 異常検出\n");
		return false;
		break;
	}
	return false;
}

ACTIVE_STATE NetWork::GetActive(void)
{
	if (!state_)
	{
		return ACTIVE_STATE::NON;
	}
	return state_->GetActive();
}

IPDATA NetWork::GetIP(void)
{
	IPDATA myIP;
	GetMyIPAddress(&myIP);
	return myIP;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{
	if (!state_)
	{
		return false;
	}
	return state_->ConnectHost(hostIP);;
}

NetWorkMode NetWork::GetMode(void)
{
	if (!state_)
	{
		return NetWorkMode::OFFLINE;
	}
	return state_->GetMode();
}

void NetWork::RecvMes(Vector2& pos)
{
	if (!state_)
	{
		return;
	}
	auto handle = state_->GetNetHandle();
	if (handle==-1)
	{
		return;
	}
	if (GetNetWorkDataLength(handle) >= sizeof(MES_DATA))
	{
		MES_DATA mes;
		NetWorkRecv(handle, &mes, sizeof(MES_DATA));
		if (mes.type == MES_TYPE::POS)
		{
			pos = { mes.data[0],mes.data[1] };
		}
	}
}

void NetWork::SendMes(MES_DATA data)
{
	if (!state_)
	{
		return;
	}
	auto handle = state_->GetNetHandle();
	if (handle == -1)
	{
		return;
	}
	NetWorkSend(handle, &data, sizeof(MES_DATA));
}

void NetWork::SendStanby(void)
{
	if (!state_)
	{
		return;
	}
	auto handle = state_->GetNetHandle();
	if (handle == -1)
	{
		return;
	}
	MES_DATA tmpMes = { MES_TYPE::STANBY,{0,0} };
	NetWorkSend(handle, &tmpMes, sizeof(MES_DATA));
	std::cout << "GUESTからのスタートメッセージ待ちです" << std::endl;
	state_->SetActive(ACTIVE_STATE::STANBY);
}

void NetWork::SendStart(void)
{
	if (!state_)
	{
		return;
	}
	auto handle = state_->GetNetHandle();
	if (handle == -1)
	{
		return;
	}
	MES_DATA tmpMes = { MES_TYPE::GAME_START,{0,0} };
	NetWorkSend(handle, &tmpMes, sizeof(MES_DATA));
}

bool NetWork::GetRevMesType(MES_TYPE type)
{
	if (!state_)
	{
		return false;
	}
	auto handle = state_->GetNetHandle();
	if (handle == -1)
	{
		return false;
	}
	if (GetNetWorkDataLength(handle) >= sizeof(MES_DATA))
	{
		MES_DATA mes;
		NetWorkRecv(handle, &mes, sizeof(MES_DATA));
		if (mes.type == type)
		{
			return true;
		}
	}
	return false;
}

bool NetWork::GetRevStanby(void)
{
	if (!state_)
	{
		return false;
	}
	auto handle = state_->GetNetHandle();
	if (handle == -1)
	{
		return false;
	}
	if (GetNetWorkDataLength(handle) >= sizeof(MES_DATA))
	{
		MES_DATA mes;
		NetWorkRecv(handle, &mes, sizeof(MES_DATA));
		if (mes.type ==MES_TYPE::STANBY)
		{
			std::cout << "スタンバイMES受信" << std::endl;
			revState_ = true;
		}
	}
	return revState_;
}

NetWork::NetWork()
{
	state_ = nullptr;
	revState_ = true;
}

NetWork::~NetWork()
{
}
