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
			
			if (mes.type == MES_TYPE::TMX_SIZE)
			{
				if (mes.length != 0)
				{
					NetWorkRecv(handle, &tmxSizeData_, mes.length * sizeof(sendData));
					TRACE("TMXのデータｻｲｽﾞは%dです。\n", tmxSizeData_.num);
					revSize_ = tmxSizeData_.num;
				}
				else
				{
					std::cout << "サイズデータの長さが0です" << std::endl;
				}
				strat_ = std::chrono::system_clock::now();
				continue;
			}
			if (mes.type == MES_TYPE::TMX_DATA)
			{
				if (mes.length != 0)
				{
					revData_.resize(mes.length);
					NetWorkRecv(handle, revData_.data(), static_cast<int>(revData_.size() * sizeof(revData_[0])));
				}
				end_ = std::chrono::system_clock::now();
				continue;
			}
			if (mes.type == MES_TYPE::STANBY)
			{
				if (mes.length != 0)
				{
					std::cout << "スタンバイにデータ部があります" << std::endl;
					MesDataVec tmpData;
					tmpData.resize(mes.length);
					NetWorkRecv(handle, tmpData.data(), static_cast<int>(tmpData.size() * sizeof(tmpData[0])));
				}
				std::cout << "受信時間" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - strat_).count() << std::endl;
				SaveTmx();
				continue;
			}
			if (mes.type==MES_TYPE::GAME_START)
			{
				if (mes.length != 0)
				{
					std::cout << "ゲームスタートにデータ部があります" << std::endl;
					MesDataVec tmpData;
					tmpData.resize(mes.length);
					NetWorkRecv(handle, tmpData.data(), static_cast<int>(tmpData.size() * sizeof(tmpData[0])));
				}
				std::lock_guard<std::mutex> lock(stMtx_);
				gameStart_ = true;
				continue;
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
	GetMyIPAddress(&ipdata_[0],static_cast<int>(ipdata_.size()));
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

void NetWork::SendMes(MesDataVec data)
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
	NetWorkSend(handle, data.data(), static_cast<int>(data.size() * sizeof(data[0])));
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
	MesDataVec mes;
	mes_H tmpMes;
	tmpMes.head = { MES_TYPE::STANBY,0,0,0 };
	mes.reserve(sizeof(tmpMes) / sizeof(sendData));
	mes.resize(sizeof(tmpMes) / sizeof(sendData));
	mes[0].idata = tmpMes.ihead[0];
	mes[1].idata = tmpMes.ihead[1];
	SendMes(mes);
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
	MesDataVec mes;
	mes_H tmpMes = { MES_TYPE::GAME_START,0,0,0 };
	mes.reserve(sizeof(tmpMes) / sizeof(sendData));
	mes.resize(sizeof(tmpMes) / sizeof(sendData));
	mes[0].idata = tmpMes.ihead[0];
	mes[1].idata = tmpMes.ihead[1];
	SendMes(mes);
}

bool NetWork::SendTmxData(std::string filename)
{
	MesDataVec sendMes;
	auto header = sizeof(mes_H) / sizeof(sendData);
	auto data = ONE_SEND_MES / sizeof(sendData);
	sendMes.reserve(data + header);
	sendMes.resize(data + header);
	std::stringstream lineData;
	std::ifstream ifp(filename.c_str());
	std::string str;
	std::string num;
	sendData sdata = {0};
	int cnt = 0;
	unsigned short id = 0;
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
		mes_H send = { MES_TYPE::TMX_DATA,0,id,static_cast<unsigned int>(sendMes.size())};
		MesDataVec mes;
		mes.resize(sizeof(send) / sizeof(sendData));
		mes[0].idata = send.ihead[0];
		mes[1].idata = send.ihead[1];
		sendMes.emplace(sendMes.begin(), mes[1]);
		sendMes.emplace(sendMes.begin(), mes[0]);
		SendMes(sendMes);

		sendMes.clear();
		sendMes.reserve(data + header);
		sendMes.resize(data + header);

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
					sdata.cdata[cnt % 8 / 2] |= tmp;
					cnt++;
					if (cnt % 8 == 0)
					{
						sendMes[(static_cast<__int64>(cnt) / 8) - 1] = sdata;
						sdata = { 0 };
					}
					if (cnt / 8 >= (ONE_SEND_MES / sizeof(sendData)))
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
	if (cnt % 8 != 0)
	{
		sendMes[cnt / 8] = sdata;
	}
	SendData();
	std::cout << id << std::endl;
	return true;
}

void NetWork::SaveTmx(void)
{
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
		return;
	}
	while (!tmx.eof())
	{
		writTmxTmp();
		if (!tmx.eof())
		{
			while (cnt < (21 * 17 * 4))
			{
				for (int bitcnt = 0; bitcnt < 8; bitcnt++)
				{
					auto num = revData_[cnt / 8].cdata[bitcnt / 2] >> (4 * (bitcnt % 2)) & 0x0f;
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
								std::cout << "スタンバイMES受信" << std::endl;
								return;
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
