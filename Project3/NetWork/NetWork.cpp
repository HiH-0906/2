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
		TRACE("NetWorkState������ �ُ팟�o\n");
		return false;
		break;
	default:
		TRACE("NetWorkState������ �ُ팟�o\n");
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
		if (mes.type == MES_TYPE::TMX_SIZE)
		{
			TRACE("TMX�̃f�[�^���ނ�%d�ł��B\n", mes.data[0]);
			revSize_ = mes.data[0];
			revTmx_.resize(mes.data[0]);
		}
		if (mes.type == MES_TYPE::TMX_DATA)
		{
			sendData data;
			data.idata[0] = mes.data[0];
			data.idata[1] = mes.data[1];
			revTmx_[mes.id] = data;
			if (mes.id==22)
			{
				auto taichi = 20;
			}
			std::cout << mes.id << std::endl;
		}
		if (mes.type == MES_TYPE::STANBY)
		{
			std::ifstream tmx("mapData/tmp.tmx");
			std::ofstream file("Capture/test.tmx", std::ios::out);
			std::string str;
			int cnt = 0;
			unsigned int mask = 15;

			if (!file)
			{
				return;
			}
			int test = 0;
			int ttt;
			int num;
			while (test < 4)
			{
				if (test==3)
				{
					auto taichi = 20;
				}
				std::getline(tmx, str);
				if (str.find("data encoding") == std::string::npos)
				{
					file << str;
					file << std::endl;
				}
				else
				{
					file << str;
					file << std::endl;
					while (true)
					{
						auto test = cnt / 16;
						if (test==21)
						{
							int nujnjj = 0;
						}
						ttt = cnt % 16 / 2;
						num = revTmx_[cnt / 16].cdata[cnt % 16 / 2] >> (4 * (cnt % 2));
						
						if (cnt % 2 == 0)
						{
							num &= mask;
						}
						std::cout << num;
						file << num;
						cnt++;
						if (cnt % (21 * 17) == 0)
						{
							file << std::endl;
							std::cout << std::endl;
							break;
						}

						if (cnt % 21 != 0)
						{
							file << ",";
							std::cout << ",";
						}
						else
						{
							file << ",";
							std::cout << ",";
							std::cout << std::endl;
							file << std::endl;
						}

					}
					test++;
				}
			}
			for (int i = 0; i < 3; i++)
			{
				std::getline(tmx, str);
				file << str;
				file << std::endl;
			}
			revState_ = true;
			std::cout << "�X�^���o�CMES��M" << std::endl;
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
	MES_DATA tmpMes = { MES_TYPE::STANBY,0,0,{0,0} };
	NetWorkSend(handle, &tmpMes, sizeof(MES_DATA));
	std::cout << "GUEST����̃X�^�[�g���b�Z�[�W�҂��ł�" << std::endl;
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
	MES_DATA tmpMes = { MES_TYPE::GAME_START,0,0,{0,0} };
	NetWorkSend(handle, &tmpMes, sizeof(MES_DATA));
}

bool NetWork::SendTmxData(std::string filename)
{
	std::stringstream lineData;
	std::ifstream ifp(filename.c_str());
	std::string str;
	std::string num;
	sendData sdata = {0};
	int cnt = 0;
	int id = 0;

	auto SetLineData = [&](std::string& str)
	{
		std::getline(ifp, str);
		lineData.str(""); // �o�b�t�@���N���A����B
		lineData.clear(std::stringstream::goodbit);
		lineData << str;
	};

	auto ChengeInt = [&](std::string str)
	{
		int num;
		std::stringstream tmp(str);
		tmp >> num;
		return num;
	};

	auto SendData = [&]()
	{
		MES_DATA send = { MES_TYPE::TMX_DATA,id,0,sdata.idata[0],sdata.idata[1] };
		SendMes(send);
		sdata = { 0 };
		id++;
	};

	if (!ifp)
	{
		return false;
	}
	while (!ifp.eof())
	{
		std::getline(ifp, str);
		if (str.find("data encoding") != std::string::npos)
		{
			SetLineData(str);
			while (str.find("</data>") == std::string::npos)
			{
				
				std::getline(lineData, num, ',');
				if (num.size() != 0)
				{
					auto tmp = ChengeInt(num);
					tmp <<= 4 * (cnt % 2);
					sdata.cdata[cnt % 16 / 2] += tmp;
					cnt++;
					if (cnt % 16 == 0)
					{
						SendData();
					}
				}
				if (lineData.eof())
				{
					SetLineData(str);
				}
			}
		}
	}
	if (cnt % 16 != 0)
	{
		SendData();
	}
	std::cout << id << std::endl;
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
