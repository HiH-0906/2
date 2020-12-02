#include <iostream>
#include <fstream>
#include <sstream>
#include "NetWork.h"
#include "HostState.h"
#include "GestState.h"
#include "../Obj/Player.h"
#include "../_debug/_DebugConOut.h"

std::unique_ptr<NetWork, NetWork::NetWorkDeleter> NetWork::s_Instance(new NetWork);



void NetWork::UpDate(void)
{
	size_t writePos = 0;
	while (!ProcessMessage())
	{
		if (!state_)
		{
			continue;
		}
		handleList_ = state_->GetNetHandle();
		if (!state_->Update())
		{
			continue;
		}
		if (state_->GetActive() == ACTIVE_STATE::INIT)
		{
			break;
		}
	}
	handleList_ = state_->GetNetHandle();
	while (!ProcessMessage() && handleList_.size())
	{
		auto tmp = GetLostNetWork();
		for (auto handle = handleList_.begin(); handle != handleList_.end(); handle++)
		{
			if (tmp == handle->first)
			{
				TRACE("ID：%dの切断", handle->second);
				sendData data;
				data = { handle->second };
				SendMesAll(MES_TYPE::LOST, MesDataList{ data }, handle->first);
				std::lock_guard<std::mutex> lock2(objRevMap_[(handle->second) / UNIT_ID_BASE].first);
				
				objRevMap_[(handle->second) / UNIT_ID_BASE].second.emplace_back(MES_H{ MES_TYPE::LOST,0,0,0 } , MesDataList{ data });
				// 殺す処理とリストから消す処理
				handleList_.erase(handle);
				if (handleList_.size())
				{
					continue;
				}
				else
				{
					break;
				}
			}
			if (GetNetWorkDataLength(handle->first) >= sizeof(MES_H))
			{
				if (mes_.next == 0)
				{
					writePos = 0;
				}
				NetWorkRecv(handle->first, &mes_, sizeof(MES_H));

				if (mes_.length != 0)
				{
					revDataList_.resize(static_cast<size_t>(writePos + mes_.length));
					NetWorkRecv(handle->first, revDataList_.data() + writePos, static_cast<int>(mes_.length * sizeof(revDataList_[0])));
					SendMesAll(mes_.type, revDataList_, handle->first);
				}

				if (mes_.next == 1)
				{
					TRACE("後続のデータがあります\n");
					writePos = revDataList_.size();
					continue;
				}
				revFunc_[static_cast<unsigned int>(mes_.type) - static_cast<unsigned int>(MES_TYPE::NON)]();
			}
		}
	}
	if (state_)
	{
		EndOfNetWork();
	}
}

void NetWork::RunUpdate(void)
{
	update_ = std::thread(&NetWork::UpDate, this);
	update_.detach();
}

void NetWork::SetObjRevData(int id, std::mutex& mtx, std::vector<RevDataP>& mes)
{
	objRevMap_.emplace_back(std::pair<std::mutex&, std::vector<RevDataP>&>{ mtx, mes });
}

bool NetWork::SetActivMode(ACTIVE_STATE state)
{
	if (!state_)
	{
		return false;
	}
	return state_->SetActive(state);
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

bool NetWork::SetCountDownTime(const std::chrono::system_clock::time_point& time)
{
	if (!state_)
	{
		return false;
	}
	state_->SetCountTime(time);
	return true;
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

void NetWork::SendMesAll(MES_TYPE type, MesDataList data)
{
	SendMesAll(type, std::move(data), 0);
}

void NetWork::SendMesAll(MES_TYPE type, MesDataList data, int noSendHandle)
{
	std::lock_guard lock(handleMtx_);
	for (auto handle = handleList_.begin(); handle != handleList_.end(); handle++)
	{
		if (handle->first != noSendHandle)
		{
			SendMes(type, data, handle->first);
		}
	}
}

void NetWork::SendMes(MES_TYPE type, MesDataList data)
{
	std::lock_guard lock(handleMtx_);
	if (handleList_.size() == 0)
	{
		return;
	}
	SendMes(type, std::move(data), handleList_.front().first);
}

void NetWork::SendMes(MES_TYPE type, MesDataList data, int handle)
{
	if (!state_)
	{
		return;
	}
	mes_H tmpMes = {};
	tmpMes.head = { type,0,0,0 };
	data.insert(data.begin(), { tmpMes.ihead[1] });
	data.insert(data.begin(), { tmpMes.ihead[0] });
	
	do
	{
		unsigned int sendCnt = static_cast<unsigned int>(data.size()) > oneSendLength_ ? oneSendLength_ : static_cast<unsigned int>(data.size());
		data[1].uidata = sendCnt - sizeof(tmpMes) / sizeof(sendData);
		// 一度に送るデータ量と送れる上限が同じなら分割している
		if (sendCnt == oneSendLength_)
		{
			/*TRACE("分割します\n");
			TRACE("送信データint数：%d\n", data[1].idata);*/
			tmpMes.head.next = 1;
		}
		else
		{
		/*	TRACE("分割しません\n");
			TRACE("送信データint数：%d\n", data[1].idata);*/
			tmpMes.head.next = 0;
		}
		data[0].uidata = tmpMes.ihead[0];
		NetWorkSend(handle, data.data(), sendCnt * sizeof(sendData));
		data.erase(data.begin() + sizeof(tmpMes) / sizeof(sendData), data.begin() + sendCnt);
		tmpMes.head.sendID++;
	} while (data.size() > (sizeof(mes_H) / sizeof(sendData)));
}

void NetWork::SendMes(MES_TYPE type,int handle)
{
	if (!state_)
	{
		return;
	}
	MesDataList data;
	SendMes(type, std::move(data),handle);
}

bool NetWork::SendTmxData(std::string filename)
{
	MesDataList sendMes;
	auto header = sizeof(mes_H) / sizeof(sendData);
	auto data = ALL_CSV_SIZE / BIT_NUM;
	if (ALL_CSV_SIZE % BIT_NUM)
	{
		data++;
	}
	sendMes.reserve(data);
	sendMes.resize(data);
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
					sdata.cdata[cnt % BIT_NUM / 2] |= tmp;
					cnt++;
					if (cnt % BIT_NUM == 0)
					{
						sendMes[(static_cast<__int64>(cnt) / BIT_NUM) - 1] = sdata;
						sdata = { 0 };
					}
				}
				if (lineData.eof())
				{
					SetLineData(str);
				}
			}
		}
	}
	if (cnt % BIT_NUM != 0)
	{
		sendMes[cnt / BIT_NUM] = sdata;
	}

	MesDataList sizeMes;
	sendData size = {};
	size.cdata[0] = 21;
	size.cdata[1] = 17;
	size.cdata[2] = 4;
	sizeMes.emplace_back(size);
	SendMesAll(MES_TYPE::TMX_SIZE, std::move(sizeMes));
	// 横縦レイヤーリザーブ
	SendMesAll(MES_TYPE::TMX_DATA, std::move(sendMes));

	std::cout << id << std::endl;
	return true;
}

bool NetWork::GetCountDownFlag(void)
{
	if (!state_)
	{
		return false;
	}
	return state_->GetCountStart();
}

const std::chrono::system_clock::time_point& NetWork::GetCountDownTime(void) const
{
	return state_->GetCountDownTime();
}

const int NetWork::GetID(void) const
{
	if (!state_)
	{
		return -1;
	}
	return state_->GetID();
}

const int NetWork::GetMax(void) const
{
	if (!state_)
	{
		return -1;
	}
	return state_->GetMax();
}

const int& NetWork::GetRevCount(void) const
{
	return revStanby_;
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
			while (cnt < ALL_CSV_SIZE)
			{
				for (int bitcnt = 0; bitcnt < 8; bitcnt++)
				{
					auto num = revDataList_[cnt / 8].cdata[bitcnt / 2] >> (4 * (bitcnt % 2)) & 0x0f;
					std::cout << num;
					file << num;
					cnt++;
					if (cnt % CSV_WIDTH != 0)
					{
						file << ",";
						std::cout << ",";
					}
					else
					{
						if (cnt % CSV_SIZE == 0)
						{
							file << std::endl;
							std::cout << std::endl;
							if (writTmxTmp())
							{
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

void NetWork::EndOfNetWork(void)
{
	for (auto handdle: state_->GetNetHandle())
	{
		CloseNetWork(handdle.first);
	}
	state_ = nullptr;
	gameStart_ = false;
	revState_ = false;
	handleList_.clear();
	objRevMap_.clear();
}

bool NetWork::GetRevStanby(void)
{
	std::lock_guard<std::mutex> lock(revMtx_);
	return revState_;
}

bool NetWork::GetGameStart(void)
{
	std::lock_guard<std::mutex> lock(stMtx_);
	return gameStart_;
}


void NetWork::RevCountDownRoom(void)
{
	if (!state_)
	{
		TRACE("RevCountDown時にNetStateがいないんですが");
		return;
	}
	unionTimeData data = { std::chrono::system_clock::now() };
	data.idata[0] = revDataList_[0].idata;
	data.idata[1] = revDataList_[1].idata;
	state_->SetCountTime(data.time);
}

void NetWork::RevID(void)
{
	if (!state_)
	{
		TRACE("RevID時にNetStateがいないんですが");
		return;
	}
	state_->SetPlayerID(revDataList_[0].idata, revDataList_[1].idata);
}

void NetWork::RevStanbyHost(void)
{
	if (mes_.length != 0)
	{
		std::cout << "スタンバイにデータ部があります" << std::endl;
	}
	revState_ = true;
	std::cout << "スタンバイMES受信" << std::endl;
	std::cout << "受信時間" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - strat_).count() << std::endl;
}

void NetWork::RevCountDownGame(void)
{
	unionTimeData time{ std::chrono::system_clock::now() };
	time.idata[0] = revDataList_[0].idata;
	time.idata[1] = revDataList_[1].idata;
	state_->SetCountTime(time.time);
	gameStart_ = true;
}

void NetWork::RevStanbyGuest(void)
{
	if (mes_.length != 0)
	{
		std::cout << "ゲームスタートにデータ部があります" << std::endl;
	}
	revStanby_++;
	if (state_->GetMax() >= revStanby_)
	{
		std::lock_guard<std::mutex> lock(stMtx_);
		gameStart_ = true;
	}
}

void NetWork::RevTmxSize(void)
{
	revSize_ = (revDataList_[0].cdata[0] * revDataList_[0].cdata[1] * revDataList_[0].cdata[2]) / 8;
	TRACE("TMXのデータｻｲｽﾞは%dです。\n", revSize_);
	strat_ = std::chrono::system_clock::now();
}

void NetWork::RevTmxData(void)
{
	end_ = std::chrono::system_clock::now();
	SaveTmx();
}

void NetWork::RevData(void)
{
	std::lock_guard<std::mutex> lock2(objRevMap_[(revDataList_[0].uidata) / UNIT_ID_BASE].first);
	objRevMap_[(revDataList_[0].uidata) / UNIT_ID_BASE].second.emplace_back(mes_, revDataList_);
}

NetWork::NetWork()
{
	revStanby_ = 0;
	state_ = nullptr;
	gameStart_ = false;
	revState_ = false;
	cntRev_ = 0;
	revFunc_[0] = nullptr;
	revFunc_[1] = (std::bind(&NetWork::RevCountDownRoom, this));
	revFunc_[2] = (std::bind(&NetWork::RevID, this));
	revFunc_[3] = (std::bind(&NetWork::RevStanbyHost, this));
	revFunc_[4] = (std::bind(&NetWork::RevStanbyGuest, this));
	revFunc_[5] = (std::bind(&NetWork::RevCountDownGame, this));
	revFunc_[6] = (std::bind(&NetWork::RevTmxSize, this));
	revFunc_[7] = (std::bind(&NetWork::RevTmxData, this));
	revFunc_[8] = (std::bind(&NetWork::RevData, this));
	revFunc_[9] = (std::bind(&NetWork::RevData, this));
	revFunc_[10] = (std::bind(&NetWork::RevData, this));
	revFunc_[11] = (std::bind(&NetWork::RevData, this));

	mes_ = {};
	revDataList_.reserve(180);
	std::ifstream inistr("ini/setting.txt");
 	std::string str;
	if (!inistr)
	{
		TRACE("Init用ファイルが読み込めません");
		oneSendLength_ = 500;
		return;
	}
	// 最大送信数の読み込み
	std::getline(inistr, str);
	do
	{
		if (str.find("byte") != std::string::npos)
		{
			auto first = str.find_first_of('"') + 1;
			auto last = str.find_last_of('"');
			str = str.substr(first, last- first);
			oneSendLength_ = std::atoi(str.c_str());
		}
		std::getline(inistr, str);
	} while (!inistr.eof());
	oneSendLength_ /= sizeof(sendData);
}

NetWork::~NetWork()
{
	if (update_.joinable())
	{
		update_.join();
	}
}
