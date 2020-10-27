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
	if (handle == -1)
	{
		return;
	}
	if (GetNetWorkDataLength(handle) >= sizeof(MES_DATA))
	{
		MES_DATA mes;
		NetWorkRecv(handle, &mes, sizeof(MES_DATA));
		if (static_cast<MES_TYPE>(mes.type) == MES_TYPE::TMX_SIZE)
		{
			TRACE("TMXのデータｻｲｽﾞは%dです。\n", mes.data[0]);
			revSize_ = mes.data[0];
			revTmx_.resize(mes.data[0] / sizeof(int));
		}
		if (static_cast<MES_TYPE>(mes.type) == MES_TYPE::TMX_DATA)
		{
			revTmx_[mes.id] = mes.data[0];
			revTmx_[mes.id + 1] = mes.data[1];
			auto tmp = reinterpret_cast<char*>(&mes.data[0]);
			std::cout << tmp[0];
			std::cout << tmp[1];
			std::cout << tmp[2];
			std::cout << tmp[3];
			auto tmp2 = reinterpret_cast<char*>(&mes.data[1]);
			std::cout << tmp2[0];
			std::cout << tmp2[1];
			std::cout << tmp2[2];
			std::cout << tmp2[3];
		}
		if (static_cast<MES_TYPE>(mes.type) == MES_TYPE::STANBY)
		{
			std::ofstream file("Capture/test.tmx", std::ios::out);

			if (!file)
			{
				return;
			}
			for (auto tmp : revTmx_)
			{
				if (!file)
				{
					return;
				}
				if (tmp != 0)
				{
					auto cha = (reinterpret_cast<char*>(&tmp));
					for (int i = 0; i < 4; i++)
					{
						if (cha[i] == 0)
						{
							continue;
						}
						file << cha[i];
					}
				}
			}
			std::cout << "スタンバイMES受信" << std::endl;
			revState_ = true;
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
	MES_DATA tmpMes = { static_cast<unsigned int>(MES_TYPE::STANBY),{},{0,0} };
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
	MES_DATA tmpMes = { static_cast<unsigned int>(MES_TYPE::GAME_START),{},{0,0} };
	NetWorkSend(handle, &tmpMes, sizeof(MES_DATA));
}

bool NetWork::SendTmxData(std::string filename)
{
	std::ifstream str(filename.c_str());
	if (!str)
	{
		return false;
	}
	char num[4];
	char num2[4];
	char tmp1, tmp2;
	int i = 0;

	while (!str.eof())
	{
		for (int i = 0; i < 4; i++)
		{
			num[i] = str.get();
		}
		for (int i = 0; i < 4; i++)
		{
			num2[i] = str.get();
		}
		MES_DATA mes = { static_cast<unsigned int>(MES_TYPE::TMX_DATA),i,*(reinterpret_cast<int*>(&num[0])),*(reinterpret_cast<int*>(&num2[0])) };
		lpNetWork.SendMes(mes);
		i += 2;
	}
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
		if (static_cast<MES_TYPE>(mes.type) == type)
		{
			return true;
		}
	}
	return false;
}

bool NetWork::GetRevStanby(void)
{
	return revState_;
}

NetWork::NetWork()
{
	state_ = nullptr;
	revState_ = false;
	cntRev_ = 0;
}

NetWork::~NetWork()
{
}
