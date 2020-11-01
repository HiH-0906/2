#include <iostream>
#include <fstream>
#include <sstream>
#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::s_Instance(new NetWork);



void NetWork::UpDate(void)
{
	while (ProcessMessage() == 0)
	{
		if (!state_)
		{
			return;
		}
		if(!state_->Update())
		{
			return;
		}
		auto handle = state_->GetNetHandle();
		if (handle == -1)
		{
			continue;
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
				strat_ = std::chrono::system_clock::now();
				continue;
			}
			if (mes.type == MES_TYPE::TMX_DATA)
			{
				sendData data;
				data.idata[0] = mes.data[0];
				data.idata[1] = mes.data[1];
				revTmx_[mes.id] = data;
				std::cout << mes.id << std::endl;
				continue;
			}
			if (mes.type == MES_TYPE::STANBY)
			{

				end_ = std::chrono::system_clock::now();
				std::cout << "��M����" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - strat_).count() << std::endl;
				std::ifstream tmx("mapData/tmp.tmx");
				std::ofstream file("Capture/test.tmx", std::ios::out);
				std::string str;
				int cnt = 0;

				auto writtem = [&]()
				{
					do
					{
						std::getline(tmx, str);
						if (tmx.eof())
						{
							return true;
						}
						file << str;
						file << std::endl;
					} while (str.find("data encoding") == std::string::npos);
					return false;
				};

				if (!file)
				{
					continue;
				}
				while (!tmx.eof())
				{
					do
					{
						std::getline(tmx, str);
						if (tmx.eof())
						{
							break;
						}
						file << str;
						file << std::endl;
					} while (str.find("data encoding") == std::string::npos);
					if (!tmx.eof())
					{
						while (cnt < (21 * 17 * 4))
						{
							for (int bitcnt = 0; bitcnt < 16; bitcnt++)
							{
								auto num = revTmx_[cnt / 16].cdata[bitcnt / 2] >> (4 * (bitcnt % 2)) & 0x0f;
								std::cout << num;
								file << num;
								cnt++;
								if (cnt % 21 != 0)
								{
									file << ",";
									std::cout << ",";
								}
								else
								{
									if (cnt % (21 * 17) == 0)
									{
										file << std::endl;
										std::cout << std::endl;
										if (writtem())
										{
											revState_ = true;
											bitcnt = 16;
											std::cout << "�X�^���o�CMES��M" << std::endl;
											continue;
										}
									}
									else
									{
										file << ",";
										std::cout << ",";
										std::cout << std::endl;
										file << std::endl;
									}
								}
							}
						}
					}
				}
			}
			if (mes.type==MES_TYPE::GAME_START)
			{
				std::lock_guard<std::mutex> lock(stMtx_);
				gameStart_ = true;
				continue;
			}
			{
				std::lock_guard<std::mutex> lock(mesMtx_);
				mesList_.push_back(mes);
			}
		}
	}
}

void NetWork::RunUpdate(void)
{
	updatae_ = std::thread(&NetWork::UpDate, this);
	updatae_.detach();
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
		RunUpdate();
		break;
	case NetWorkMode::GEST:
		state_ = std::make_unique<GestState>();
		RunUpdate();
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
	strat_ = std::chrono::system_clock::now();
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
					sdata.cdata[cnt % 16 / 2] |= tmp;
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

bool NetWork::GetRevStanby(void)
{
	return revState_;
}

bool NetWork::GetGameStart(void)
{
	std::lock_guard<std::mutex> lock(stMtx_);
	return gameStart_;
}

MES_DATA NetWork::PickUpMes(void)
{
	std::lock_guard<std::mutex> lock(mesMtx_);
	if (mesList_.size() == 0)
	{
		return {};
	}
	auto mes = mesList_.begin();
	mesList_.pop_front();

	return *mes;
}

NetWork::NetWork()
{
	state_ = nullptr;
	gameStart_ = false;
	revState_ = false;
	cntRev_ = 0;
}

NetWork::~NetWork()
{
}
