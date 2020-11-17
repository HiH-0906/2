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
	size_t writePos = 0;
	while (!ProcessMessage())
	{
		if (!state_)
		{
			continue;
		}
		if (!state_->Update())
		{
			continue;
		}
		handle_ = state_->GetNetHandle();
		if (handle_ != -1)
		{
			break;
		}
	}
	
	while (!ProcessMessage() && GetLostNetWork() == -1)
	{
		if (GetNetWorkDataLength(handle_) >= sizeof(MES_H))
		{
			if (mes_.next == 0)
			{
				writePos = 0;
			}
			NetWorkRecv(handle_, &mes_, sizeof(MES_H));
			
			if (mes_.length != 0)
			{
				revData_.resize(static_cast<size_t>(writePos + mes_.length));
				NetWorkRecv(handle_, revData_.data() + writePos, static_cast<int>(mes_.length * sizeof(revData_[0])));
			}

			if (mes_.next == 1)
			{
				TRACE("後続のデータがあります\n");
				writePos = revData_.size();
				continue;
			}
			revFunc_[static_cast<unsigned int>(mes_.type) - static_cast<unsigned int>(MES_TYPE::NON)]();
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

void NetWork::SetObjRevData(int id, std::mutex& mtx, std::vector<RevData>& mes)
{
	objRevMap_.emplace_back(std::pair<std::mutex&, std::vector<RevData>&>{ mtx, mes });
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

void NetWork::SendMes(MES_TYPE type, MesDataList data)
{
	if (!state_)
	{
		return;
	}
	if (handle_ == -1)
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
		data[1].idata = sendCnt - sizeof(tmpMes) / sizeof(sendData);
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
		data[0].idata = tmpMes.ihead[0];
		NetWorkSend(handle_, data.data(), sendCnt * sizeof(sendData));
		data.erase(data.begin() + sizeof(tmpMes) / sizeof(sendData), data.begin() + sendCnt);
		tmpMes.head.sendID++;
	} while (data.size() > (sizeof(mes_H) / sizeof(sendData)));
}

void NetWork::SendMes(MES_TYPE type)
{
	if (!state_)
	{
		return;
	}
	MesDataList data;
	SendMes(type, std::move(data));
}

void NetWork::SendStanby(void)
{
	if (!state_)
	{
		return;
	}
	SendMes(MES_TYPE::STANBY);
	std::cout << "GUESTからのスタートメッセージ待ちです" << std::endl;
	state_->SetActive(ACTIVE_STATE::STANBY);
}

void NetWork::SendStart(void)
{
	if (!state_)
	{
		return;
	}
	SendMes(MES_TYPE::GAME_START);
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
	SendMes(MES_TYPE::TMX_SIZE, std::move(sizeMes));
	// 横縦レイヤーリザーブ
	SendMes(MES_TYPE::TMX_DATA, std::move(sendMes));

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
			while (cnt < ALL_CSV_SIZE)
			{
				for (int bitcnt = 0; bitcnt < 8; bitcnt++)
				{
					auto num = revData_[cnt / 8].cdata[bitcnt / 2] >> (4 * (bitcnt % 2)) & 0x0f;
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

void NetWork::EndOfNetWork(void)
{
	CloseNetWork(state_->GetNetHandle());
	state_ = nullptr;
	gameStart_ = false;
	revState_ = false;
	handle_ = -1;
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


void NetWork::RevStanby(void)
{
	if (mes_.length != 0)
	{
		std::cout << "スタンバイにデータ部があります" << std::endl;
		MesDataList tmpData;
		tmpData.resize(mes_.length);
		NetWorkRecv(handle_, tmpData.data(), static_cast<int>(tmpData.size() * sizeof(tmpData[0])));
	}
	std::cout << "受信時間" << std::chrono::duration_cast<std::chrono::milliseconds>(end_ - strat_).count() << std::endl;

}

void NetWork::RevGameStart(void)
{
	if (mes_.length != 0)
	{
		std::cout << "ゲームスタートにデータ部があります" << std::endl;
		MesDataList tmpData;
		tmpData.resize(mes_.length);
		NetWorkRecv(handle_, tmpData.data(), static_cast<int>(tmpData.size() * sizeof(tmpData[0])));
	}
	std::lock_guard<std::mutex> lock(stMtx_);
	gameStart_ = true;
}

void NetWork::RevTmxSize(void)
{

	revSize_ = (revData_[0].cdata[0] * revData_[0].cdata[1] * revData_[0].cdata[2]) / 8;
	TRACE("TMXのデータｻｲｽﾞは%dです。\n", revSize_);
	strat_ = std::chrono::system_clock::now();
}

void NetWork::RevTmxData(void)
{
	end_ = std::chrono::system_clock::now();
	SaveTmx();
}

void NetWork::RevPos(void)
{
	std::lock_guard<std::mutex> lock2(objRevMap_[revData_[0].idata].first);
	objRevMap_[revData_[0].idata].second.emplace_back(mes_, revData_);
}

NetWork::NetWork()
{
	state_ = nullptr;
	gameStart_ = false;
	revState_ = false;
	cntRev_ = 0;
	handle_ = -1;
	revFunc_[0] = nullptr;
	revFunc_[1]=(std::bind(&NetWork::RevStanby, this));
	revFunc_[2] = (std::bind(&NetWork::RevGameStart, this));
	revFunc_[3] = (std::bind(&NetWork::RevTmxSize, this));
	revFunc_[4] = (std::bind(&NetWork::RevTmxData, this));
	revFunc_[5] = (std::bind(&NetWork::RevPos, this));

	mes_ = {};
	revData_.reserve(180);
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
