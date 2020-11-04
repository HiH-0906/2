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
	while (!ProcessMessage())
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
		if (GetNetWorkDataLength(handle) >= sizeof(MES_H))
		{
			MES_H mes;
			NetWorkRecv(handle, &mes, sizeof(MES_H));
			while (GetNetWorkDataLength(handle) < mes.length)
			{
				// データ部受信待機
			}
			if (mes.type == MES_TYPE::TMX_SIZE)
			{
				NetWorkRecv(handle, &tmxSizeData_, mes.length);
				TRACE("TMXのデータｻｲｽﾞは%dです。\n", tmxSizeData_.size);
				revSize_ = tmxSizeData_.size;
				revTmx_.resize(tmxSizeData_.size);
				strat_ = std::chrono::system_clock::now();
				continue;
			}
			if (mes.type == MES_TYPE::TMX_DATA)
			{
				sendData data;
				int cnt = 0;
				while (cnt < tmxSizeData_.size)
				{
					NetWorkRecv(handle, &data, sizeof(sendData));
					revTmx_[mes.id] = data;
					std::cout << mes.id << std::endl;
					cnt += sizeof(sendData);
					if (cnt >= tmxSizeData_.allsize)
					{
						break;
					}
				}
				continue;
			}
			if (mes.type == MES_TYPE::STANBY)
			{

				end_ = std::chrono::system_clock::now();
				std::cout << "受信時間" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - strat_).count() << std::endl;
				std::ifstream tmx("mapData/tmp.tmx");
				std::ofstream file("Capture/test.tmx", std::ios::out);
				std::string str;
				int cnt = 0;

				auto writTmxTmp = [&]()
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
					writTmxTmp();
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
										if (writTmxTmp())
										{
											revState_ = true;
											bitcnt = 16;
											std::cout << "スタンバイMES受信" << std::endl;
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
	if (state_)
	{
		CloseNetWork(state_->GetNetHandle());
	}
}

void NetWork::RunUpdate(void)
{
	update_ = std::thread(&NetWork::UpDate, this);
	update_.detach();
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

void NetWork::SendMes(MES_H data)
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
	NetWorkSend(handle, &data, sizeof(MES_H));
}

void NetWork::SendTmxSize(TMX_SIZE data)
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
	NetWorkSend(handle, &data, sizeof(TMX_SIZE));
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
	MES_H tmpMes = { MES_TYPE::STANBY,0,0,0 };
	NetWorkSend(handle, &tmpMes, sizeof(MES_H));
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
	MES_H tmpMes = { MES_TYPE::GAME_START,0,0,0 };
	NetWorkSend(handle, &tmpMes, sizeof(MES_H));
}

bool NetWork::SendTmxData(std::string filename)
{
	
	std::stringstream lineData;
	std::ifstream ifp(filename.c_str());
	std::string str;
	std::string num;
	sendData sdata = {0};
	std::array<sendData, (1000 / sizeof(sendData))> test;
	int cnt = 0;
	int id = 0;
	strat_ = std::chrono::system_clock::now();
	auto SetLineData = [&](std::string& str)
	{
		std::getline(ifp, str);
		lineData.str(""); // バッファをクリアする。
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
		MES_H send = { MES_TYPE::TMX_DATA,id,0,sdata.idata[0],sdata.idata[1] };
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

MES_H NetWork::PickUpMes(void)
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
	if (update_.joinable())
	{
		update_.join();
	}
}
