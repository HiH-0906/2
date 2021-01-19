#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
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
		{
			std::lock_guard<std::mutex> lock(handleMtx_);
			for (auto handle = handleList_.begin(); handle != handleList_.end(); handle++)
			{
				if (tmp == handle->handle)
				{
					TRACE("ID�F%d�̐ؒf\n", handle->id);
					handle->state = -1;
				}
				if (handle->state == -1 && playNow_)
				{
					sendData data;
					data = { handle->id };
					SendMesAll(MES_TYPE::LOST, MesDataList{ data }, handle->handle);
					{
						std::lock_guard<std::mutex> lock(objRevMtx_);
						std::lock_guard<std::mutex> lock2(objRevMap_[(handle->id) / UNIT_ID_BASE].first);
						objRevMap_[(handle->id) / UNIT_ID_BASE].second.emplace_back(MES_H{ MES_TYPE::LOST,0,0,0 }, MesDataList{ data });
					}
					// �E�������ƃ��X�g�����������
					handleList_.erase(handle);
					break;
				}
				if (GetNetWorkDataLength(handle->handle) >= sizeof(MES_H))
				{
					if (mes_.next == 0)
					{
						writePos = 0;
					}

					int sendID = 0;

					do
					{
						NetWorkRecv(handle->handle, &mes_, sizeof(MES_H));
						if (static_cast<unsigned int>(mes_.type) < static_cast<unsigned int>(MES_TYPE::NON) ||
							static_cast<unsigned int>(mes_.type) > static_cast<unsigned int>(MES_TYPE::MAX))
						{
							TRACE("�����Ă���MES��TYPE�ُ�\n");
							continue;
						}
						if (mes_.length != 0)
						{
							revDataList_.resize(static_cast<size_t>(writePos + mes_.length));
							NetWorkRecv(handle->handle, revDataList_.data() + writePos, static_cast<int>(mes_.length * sizeof(revDataList_[0])));
							SendMesAll(mes_.type, revDataList_, handle->handle);
						}
						if (mes_.sendID != sendID)
						{
							TRACE("�����Ă���MES��SendID�ُ�\n");
							continue;
						}
						if (oldType_ != mes_.type)
						{
							TRACE("�㑱��MES_TYPE���Ⴂ�܂�\n");
							TRACE("OLD:%d\n",oldType_);
							TRACE("NEW:%d\n",mes_.type);
							writePos = 0;
							oldType_ = {};
							continue;
						}
						if (mes_.next == 1)
						{
							TRACE("�㑱�̃f�[�^������܂�\n");
							oldType_ = mes_.type;
							writePos = revDataList_.size();
							sendID++;
						}
					} while (mes_.next);
					if (static_cast<unsigned int>(mes_.type) > static_cast<unsigned int>(MES_TYPE::NON) &&
						static_cast<unsigned int>(mes_.type) < static_cast<unsigned int>(MES_TYPE::MAX))
					{
						const auto type = static_cast<unsigned int>(mes_.type) - static_cast<unsigned int>(MES_TYPE::NON);
						if (checkData_[mes_.type] == revDataList_.size())
						{
							revFunc_[type](handle);
						}
						revDataList_.resize(0);
						mes_ = {};
					}
					oldType_ = MES_TYPE::MAX;
				}
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
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		objRevMap_.emplace_back(std::pair<std::mutex&, std::vector<RevDataP>&>{ mtx, mes });
	}
}

void NetWork::SetPlayNow(bool flag)
{
	std::lock_guard<std::mutex> lock(nowMtx_);
	playNow_ = flag;
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

bool NetWork::SetCountDownRoomTime(const std::chrono::system_clock::time_point& time)
{
	if (!state_)
	{
		return false;
	}
	state_->SetCountDownRoomTime(time);
	return true;
}

bool NetWork::SetCountDownGameTime(const std::chrono::system_clock::time_point& time)
{
	if (!state_)
	{
		return false;
	}
	state_->SetGameStart(true);
	state_->SetCountDownGameTime(time);
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

void NetWork::SendResult(std::list<int>& data)
{
	for (int tmp = 0; data.size() < 5; tmp++)
	{
		data.emplace_back(-1);
	}
	SetResult(data);
	if (!state_)
	{
		return;
	}
	if (state_->GetMode() == NetWorkMode::HOST)
	{
		sendData send[5] = {};

		int i = 0; 
		for (auto result : data)
		{
			send[i].idata = result;
			i++;
		}

		SendMesAll(MES_TYPE::RESULT, MesDataList{ send[0],send[1],send[2],send[3],send[4] });
	}
}

void NetWork::SendMesAll(MES_TYPE type, MesDataList data)
{
	SendMesAll(type, std::move(data), 0);
}

void NetWork::SendMesAll(MES_TYPE type, MesDataList data, int noSendHandle)
{
	//std::lock_guard<std::mutex> lock(handleMtx_);
	if (handleList_.size() == 0)
	{
		return;
	}
	for (auto handle = handleList_.begin(); handle != handleList_.end(); handle++)
	{
		if (handle->handle != noSendHandle)
		{
			SendMes(type, data, handle->handle);
		}
	}
}

void NetWork::SendMes(MES_TYPE type, MesDataList data)
{
	//std::lock_guard lock(handleMtx_);
	if (handleList_.size() == 0)
	{
		return;
	}
	SendMes(type, std::move(data), handleList_.front().handle);
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
		// ��x�ɑ���f�[�^�ʂƑ�������������Ȃ番�����Ă���
		if (sendCnt == oneSendLength_)
		{
			/*TRACE("�������܂�\n");
			TRACE("���M�f�[�^int���F%d\n", data[1].idata);*/
			tmpMes.head.next = 1;
		}
		else
		{
		/*	TRACE("�������܂���\n");
			TRACE("���M�f�[�^int���F%d\n", data[1].idata);*/
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

void NetWork::SetStartGame(bool flag)
{
	std::lock_guard<std::mutex> lock(stMtx_);
	startGame_ = flag;
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
	// ���c���C���[���U�[�u
	SendMesAll(MES_TYPE::TMX_DATA, std::move(sendMes));

	TRACE("%d\n",id);
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

const std::chrono::system_clock::time_point NetWork::GetCountDownRoomTime(void) const
{
	if (!state_)
	{
		std::chrono::system_clock::time_point time;
		return time;
	}
	return state_->GetCountDownRoomTime();
}

const std::chrono::system_clock::time_point NetWork::GetCountDownGameTime(void) const
{
	if (!state_)
	{
		std::chrono::system_clock::time_point time;
		return time;
	}
	return state_->GetCountDownGameTime();
}

const int NetWork::GetStanbyPlayerNum(void) const
{
	return static_cast<int>(std::count_if(handleList_.begin(), handleList_.end(), [](HANDLE_DATA handle) {return handle.state == 1; }));
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

const std::list<int>& NetWork::GetResult(void)
{
	std::lock_guard<std::mutex> lock(resultMtx_);
	return resultData_;
}

void NetWork::SetResult(const std::list<int>& resultData)
{
	std::lock_guard<std::mutex> lock(resultMtx_);
	if (!revResult_)
	{
		resultData_ = resultData;
	}
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
					if (num < 0 || num>12)
					{
						TRACE("�����Ă���TMX�̒l��%d�������̂�0�ɕ␳\n", num);
						num = 0;
					}
					TRACE("%d", num);
					file << num;
					cnt++;
					if (cnt % CSV_WIDTH != 0)
					{
						file << ",";
						TRACE(",");
					}
					else
					{
						if (cnt % CSV_SIZE == 0)
						{
							file << std::endl;
							TRACE("\n");
							if (writTmxTmp())
							{
								return;
							}
						}
						else
						{
							file << ",";
							TRACE(",\n");
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
	if (state_)
	{
		for (const auto& handdle : state_->GetNetHandle())
		{
			CloseNetWork(handdle.handle);
		}
	}
	state_ = nullptr;
	{
		std::lock_guard <std::mutex> lock(stMtx_);
		startGame_ = false;
	}
	{
		std::lock_guard<std::mutex> lock(revIDMtx_);
		revID_ = false;
	}
	{
		std::lock_guard< std::mutex> lock(revMtx_);
		revState_ = false;
	}
	{
		std::lock_guard < std::mutex> lock(nowMtx_);
		playNow_ = false;
	}
	{
		std::lock_guard<std::mutex> lock(handleMtx_);
		handleList_.clear();
	}
	{
		std::lock_guard<std::mutex> lock(resultMtx_);
		revResult_ = false;
		resultData_.clear();
	}
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		objRevMap_.clear();
	}
	oldType_ = MES_TYPE::MAX;
}

bool NetWork::GetRevStanby(void)
{
	std::lock_guard<std::mutex> lock(revMtx_);
	return revState_;
}

bool NetWork::GetStartGame(void)
{
	std::lock_guard<std::mutex> lock(stMtx_);
	return startGame_;
}

bool NetWork::GetRevResult(void)
{
	return revResult_;
}


void NetWork::RevNonMes(HandleList::iterator& itr)
{
	TRACE("NON_MES��M\n");
}

void NetWork::RevCountDownRoom(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����CountDownRoom�f�[�^��M\n");
			return;
		}
	}
	if (!state_)
	{
		TRACE("RevCountDown����NetState�����Ȃ���ł���\n");
		return;
	}
	if (state_->GetCountStart())
	{
		TRACE("RevCountDownRoom�Ă�\n");
		return;
	}
	TRACE("RevCountDownRoom��M\n");
	unionTimeData data = { std::chrono::system_clock::now() };
	data.idata[0] = revDataList_[0].idata;
	data.idata[1] = revDataList_[1].idata;
	state_->SetCountDownRoomTime(data.time);
}

void NetWork::RevID(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����ID�f�[�^��M\n");
			return;
		}
	}
	if (!state_)
	{
		TRACE("RevID����NetState�����Ȃ���ł���\n");
		return;
	}
	if (revID_)
	{
		TRACE("RevID��2���\n");
		TRACE("PlayerID�F%d	PlayerMax�F%d\n", revDataList_[0].idata, revDataList_[1].idata);
		return;
	}
	auto tmpID = revDataList_[0].idata % UNIT_ID_BASE;
	if (tmpID != 0)
	{
		TRACE("PlayerID�ُ�F%d\n", revDataList_[0].idata);
		TRACE("PlayerID�F%d	PlayerMax�F%d\n", revDataList_[0].idata, revDataList_[1].idata);
		return;
	}
	if (revDataList_[0].idata >= revDataList_[1].idata * UNIT_ID_BASE || revDataList_[0].idata < 0)
	{
		TRACE("PlayerID�ُ�F%d\n", revDataList_[0].idata);
		TRACE("PlayerID�F%d	PlayerMax�F%d\n", revDataList_[0].idata, revDataList_[1].idata);
		return;
	}
	TRACE("PlayerID�F%d	PlayerMax�F%d\n", revDataList_[0].idata, revDataList_[1].idata);
	state_->SetPlayerID(revDataList_[0].idata, revDataList_[1].idata);
	{
		std::lock_guard<std::mutex> lock(revIDMtx_);
		revID_ = true;
	}
}

void NetWork::RevStanbyHost(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����StanbyHost�f�[�^��M\n");
			return;
		}
	}
	if (mes_.length != 0)
	{
		TRACE("�X�^���o�C�Ƀf�[�^��������܂�");
	}
	revState_ = true;
	TRACE("�X�^���o�CMES��M");
}

void NetWork::RevCountDownGame(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����CountDownGame�f�[�^��M\n");
			return;
		}
	}
	if ((!revState_))
	{
		TRACE("�X�^���o�CMES�OCountDownGame�f�[�^��M\n");
		return;
	}
	if (revState_ && startGame_)
	{
		TRACE("2�x��CountDownGame�f�[�^��M\n");
		return;
	}
	TRACE("CountDownGame�f�[�^��M\n");
	unionTimeData time{ std::chrono::system_clock::now() };
	time.idata[0] = revDataList_[0].idata;
	time.idata[1] = revDataList_[1].idata;
	state_->SetCountDownGameTime(time.time);
	startGame_ = true;
}

void NetWork::RevStanbyGuest(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����stanbyGuest�f�[�^��M\n");
			return;
		}
	}
	if (mes_.length != 0)
	{
		TRACE("�Q�[���X�^�[�g�Ƀf�[�^��������܂�");
	}
	itr->state = 1;
}

void NetWork::RevTmxSize(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����TmxSize�f�[�^��M\n");
			return;
		}
	}
	if (revDataList_[0].cdata[2] != 4)
	{
		TRACE("TmxSizeRayer�f�[�^�ُ�F%d\n", revDataList_[0].cdata[2]);
		return;
	}
	revSize_ = (revDataList_[0].cdata[0] * revDataList_[0].cdata[1] * revDataList_[0].cdata[2]) / 8;
	TRACE("TMX�̃f�[�^���ނ�%d�ł��B\n", revSize_);
	checkData_[MES_TYPE::TMX_DATA] = revSize_;
	strat_ = std::chrono::system_clock::now();
}

void NetWork::RevTmxData(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (playNow_)
		{
			TRACE("�Q�[����Tmx�f�[�^��M\n");
			return;
		}
	}
	end_ = std::chrono::system_clock::now();
	SaveTmx();
}

void NetWork::RevPosData(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (!playNow_)
		{
			TRACE("PosData �܂��n�܂��ĂȂ���\n");
			return;
		}
	}
	auto tmpID = (revDataList_[0].uidata) / UNIT_ID_BASE;
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		if (tmpID < objRevMap_.size() && tmpID >= 0)
		{
			if ((revDataList_[0].uidata) != itr->id && state_->GetMode() == NetWorkMode::HOST)
			{
				TRACE("���l��ID��PosMes ID�F%d\n", (revDataList_[0].uidata));
				return;
			}
			std::lock_guard<std::mutex> lock2(objRevMap_[tmpID].first);
			objRevMap_[tmpID].second.emplace_back(mes_, revDataList_);
		}
		else
		{
			TRACE("PosRev��ID�ُ� ID�F%d\n", tmpID);
		}
	}
}

void NetWork::RevSetBombData(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (!playNow_)
		{
			TRACE("BombData �܂��n�܂��ĂȂ���\n");
			return;
		}
	}
	auto tmpID = (revDataList_[0].uidata) / UNIT_ID_BASE;
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		if (tmpID < objRevMap_.size() && tmpID >= 0)
		{
			if ((revDataList_[0].uidata) != itr->id && state_->GetMode() == NetWorkMode::HOST)
			{
				TRACE("���l��ID��BombMes ID�F%d\n", (tmpID));
				return;
			}
			std::lock_guard<std::mutex> lock2(objRevMap_[tmpID].first);
			objRevMap_[tmpID].second.emplace_back(mes_, revDataList_);
			TRACE("BombRev ID�F%d\n", revDataList_[0].uidata);
		}
		else
		{
			TRACE("BombRev��ID�ُ� ID�F%d\n", revDataList_[0].uidata);
		}
	}
}

void NetWork::RevDethData(HandleList::iterator& itr)
{
	{
		std::lock_guard<std::mutex> lock(nowMtx_);
		if (!playNow_)
		{
			TRACE("DethData �܂��n�܂��ĂȂ���\n");
			return;
		}
	}
	auto tmpID = (revDataList_[0].uidata) / UNIT_ID_BASE;
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		if (tmpID < objRevMap_.size() && tmpID >= 0)
		{
			if ((revDataList_[0].uidata) != itr->id && state_->GetMode() == NetWorkMode::HOST)
			{
				TRACE("���l��ID��dethMes ID�F%d\n", (revDataList_[0].uidata));
				return;
			}
			std::lock_guard<std::mutex> lock2(objRevMap_[tmpID].first);
			objRevMap_[tmpID].second.emplace_back(mes_, revDataList_);
		}
		else
		{
			TRACE("DethRev��ID�ُ� ID�F%d\n", tmpID);
		}
	}
}

void NetWork::RevResultData(HandleList::iterator& itr)
{
	if (revResult_)
	{
		TRACE("2�x�ڂ�ResultData\n");
		return;
	}
	{
		std::lock_guard<std::mutex> lock(resultMtx_);
		int cnt = 0;
		for (auto data : revDataList_)
		{
			if (data.idata % UNIT_ID_BASE != 0 && data.idata != -1)
			{
				TRACE("ResultMes�f�[�^�ُ�F%d\n", data.idata);
				cnt++;
				continue;
			}
			resultData_.push_back(data.idata);
		}
		for (int i = 0; i < cnt; i++)
		{
			resultData_.emplace_back(-1);
		}
		revResult_ = true;
	}
	TRACE("ResultData�̎�M\n");
}

void NetWork::RevLostData(HandleList::iterator& itr)
{
	auto tmpID = (revDataList_[0].uidata) / UNIT_ID_BASE;
	{
		std::lock_guard<std::mutex> lock(objRevMtx_);
		if (tmpID < objRevMap_.size() && tmpID >= 0)
		{
			std::lock_guard<std::mutex> lock2(objRevMap_[tmpID].first);
			objRevMap_[tmpID].second.emplace_back(mes_, revDataList_);
		}
		else
		{
			TRACE("LostRev��ID�ُ�\n");
		}
	}
}

NetWork::NetWork()
{
	state_ = nullptr;
	startGame_ = false;
	revState_ = false;
	revResult_ = false;
	oldType_ = MES_TYPE::MAX;
	revID_ = false;
	// �������̑㏞ �킩�肸�炭�Ȃ���
	revFunc_[0] = (std::bind(&NetWork::RevNonMes, this, std::placeholders::_1));
	revFunc_[1] = (std::bind(&NetWork::RevCountDownRoom, this, std::placeholders::_1));
	revFunc_[2] = (std::bind(&NetWork::RevID, this, std::placeholders::_1));
	revFunc_[3] = (std::bind(&NetWork::RevStanbyHost, this, std::placeholders::_1));
	revFunc_[4] = (std::bind(&NetWork::RevStanbyGuest, this, std::placeholders::_1));
	revFunc_[5] = (std::bind(&NetWork::RevCountDownGame, this, std::placeholders::_1));
	revFunc_[6] = (std::bind(&NetWork::RevTmxSize, this, std::placeholders::_1));
	revFunc_[7] = (std::bind(&NetWork::RevTmxData, this, std::placeholders::_1));
	revFunc_[8] = (std::bind(&NetWork::RevPosData, this, std::placeholders::_1));
	revFunc_[9] = (std::bind(&NetWork::RevSetBombData, this, std::placeholders::_1));
	revFunc_[10] = (std::bind(&NetWork::RevDethData, this, std::placeholders::_1));
	revFunc_[11] = (std::bind(&NetWork::RevResultData, this, std::placeholders::_1));
	revFunc_[12] = (std::bind(&NetWork::RevLostData, this, std::placeholders::_1));

	mes_ = {};
	// �������̂��߂̃������Ĕz�u�h�~�p �ő�179�f�[�^�����ł���͂��Ȃ̂ŗ]�T��������200
	revDataList_.reserve(200);

	std::ifstream inistr("ini/setting.txt");
 	std::string str;
	if (!inistr)
	{
		TRACE("Init�p�t�@�C�����ǂݍ��߂܂���");
		// �l�͂���Ȃ��񂩂Ȃ���
		oneSendLength_ = 500;
		return;
	}
	// �ő呗�M���̓ǂݍ���
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

	// ���̃f�[�^�T�C�Y�ő����Ă���͂�
	// �Ȃ�TMX_DATA�̂ݎ�M����TMX_SIZE�ɂ��ς��
	checkData_.try_emplace(MES_TYPE::NON, -1);
	checkData_.try_emplace(MES_TYPE::COUNT_DOWN_GAME, 2);
	checkData_.try_emplace(MES_TYPE::COUNT_DOWN_ROOM, 2);
	checkData_.try_emplace(MES_TYPE::DETH, 1);
	checkData_.try_emplace(MES_TYPE::ID, 2);
	checkData_.try_emplace(MES_TYPE::LOST, 1);
	checkData_.try_emplace(MES_TYPE::MAX, 0);
	checkData_.try_emplace(MES_TYPE::POS, 4);
	checkData_.try_emplace(MES_TYPE::SET_BOMB, 7);
	checkData_.try_emplace(MES_TYPE::STANBY_GUEST,0 );
	checkData_.try_emplace(MES_TYPE::STANBY_HOST, 0);
	checkData_.try_emplace(MES_TYPE::TMX_DATA, 179);
	checkData_.try_emplace(MES_TYPE::TMX_SIZE, 1);
	checkData_.try_emplace(MES_TYPE::RESULT, 5);
}

NetWork::~NetWork()
{
	if (update_.joinable())
	{
		// �I���ҋ@
		update_.join();
	}
}
