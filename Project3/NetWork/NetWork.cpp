#include <iostream>
#include <fstream>
#include <sstream>
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

IParray NetWork::GetIP(void)
{
	GetMyIPAddress(&ipdata_[0],ipdata_.size());
	return ipdata_;
}

bool NetWork::ConnectHost(IPDATA hostIP)
{
	if (!state_)
	{
		return false;
	}
	return state_->ConnectHost(hostIP);
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
		if (mes.type == MES_TYPE::TMX_SIZE)
		{
			TRACE("TMXのデータｻｲｽﾞは%dです。\n", mes.data[0]);
			revSize_ = mes.data[0];
			revTmx_.resize(mes.data[0]);
		}
		if (mes.type == MES_TYPE::POS)
		{
			pos = { mes.data[0],mes.data[1] };
		}
		if (mes.type == MES_TYPE::TMX_DATA)
		{
			revTmx_[mes.data[0]] = mes.data[1];
			TRACE("%c", revTmx_[mes.data[0]])
			cntRev_++;
			if (cntRev_ == revSize_)
			{
				std::fstream file("test.tmx", std::ios::out);

				if (!file)
				{
					return;
				}
				for (auto tmp: revTmx_)
				{

					file.write((char*)&tmp, sizeof(tmp));
				}
				file.close();
			}

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

bool NetWork::SendTmxData(std::string filename)
{
	std::ifstream str(filename.c_str());
	if (!str)
	{
		return false;
	}

	char tmp = str.get();
	int i = 0;
	while (tmp != EOF)
	{
		MES_DATA mes = { MES_TYPE::TMX_DATA,i,tmp };
		lpNetWork.SendMes(mes);
		i++;
		tmp = str.get();
	}
	MES_DATA mes = { MES_TYPE::TMX_DATA,i,INT_MAX };
	lpNetWork.SendMes(mes);
	return true;
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
	cntRev_ = 0;
}

NetWork::~NetWork()
{
}
