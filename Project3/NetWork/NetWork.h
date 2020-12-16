#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include <list>
#include <chrono>
#include <utility>
#include <map>
#include <DxLib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

/// <summary>
/// �f�[�^�󂯎�莞�̒萔
/// </summary>
#define CSV_HEIGHT 17
#define CSV_WIDTH 21
#define CSV_SIZE (CSV_HEIGHT*CSV_WIDTH)
#define CSV_LAYER 4
#define ALL_CSV_SIZE (CSV_SIZE*CSV_LAYER)

#define REV_FUNC_SIZE (static_cast<size_t>(MES_TYPE::MAX) - static_cast<int>(MES_TYPE::NON))

#define BIT_NUM 8

using MesDataList = std::vector<sendData>;
using IParray = std::array<IPDATA, 5>;
using RevDataP = std::pair<MES_H, MesDataList>;
using ObjRevMap = std::vector< std::pair<std::mutex&, std::vector<RevDataP>&>>;


// �����̃V���O���g���N���X
class NetWork
{
public:
	static NetWork& GetInstance()
	{
		return *s_Instance;
	}

	void UpDate(void);
	void RunUpdate(void);

	void SetObjRevData(int id, std::mutex& mtx, std::vector<RevDataP>& mes);
	void SetPlayNow(bool flag);

	bool SetActivMode(ACTIVE_STATE state);
	bool SetNetWorkMode(NetWorkMode mode);
	bool SetCountDownRoomTime(const std::chrono::system_clock::time_point& time);
	bool SetCountDownGameTime(const std::chrono::system_clock::time_point& time);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// �w�肳�ꂽIP�A�h���X�̃z�X�g�֐ڑ����ɍs��
	NetWorkMode GetMode(void);

	void SendResult(std::list<int>& data);
	void SendMesAll(MES_TYPE type, MesDataList data);
	void SendMesAll(MES_TYPE type, MesDataList data,int noSendHandle);
	void SendMes(MES_TYPE type, MesDataList data);
	void SendMes(MES_TYPE type, MesDataList data,int handle);
	void SendMes(MES_TYPE type,int handle);
	void SetStartGame(bool flag);
	bool SendTmxData(std::string filename);

	bool GetCountDownFlag(void);
	const std::chrono::system_clock::time_point GetCountDownRoomTime(void)const;
	const std::chrono::system_clock::time_point GetCountDownGameTime(void)const;

	const int GetStanbyPlayerNum(void)const;
	const int GetID(void)const;
	const int GetMax(void)const;

	const std::list<int>& GetResult(void);
	void SetResult(const std::list<int>& resultData);

	bool GetRevStanby(void);
	bool GetStartGame(void);
	bool GetRoundEnd(void);

	void SaveTmx(void);

	void EndOfNetWork(void);


private:
	bool revState_;
	bool startGame_;
	bool playNow_;
	bool revID_;
	bool roundEnd_;
	MesDataList revDataList_;
	MES_H mes_;
	int revSize_;
	int revStanby_;
	HandleList handleList_;

	unsigned int oneSendLength_;

	IParray ipdata_;

	TMX_SIZE tmxSizeData_;

	struct NetWorkDeleter
	{
		void operator()(NetWork* net)
		{
			delete net;
		}
	};
	static std::unique_ptr<NetWork, NetWorkDeleter> s_Instance;

	// �l�b�g���[�N�֌W�{��
	std::unique_ptr<NetWorkState> state_;

	// ���V�[�u�������o�֐��|�C���^
	using RevFunc = std::function<void(HandleList::iterator& itr)>;
	std::array<RevFunc, REV_FUNC_SIZE > revFunc_;

	void RevCountDownRoom(HandleList::iterator& itr);
	void RevID(HandleList::iterator& itr);
	void RevStanbyHost(HandleList::iterator& itr);
	void RevStanbyGuest(HandleList::iterator& itr);
	void RevCountDownGame(HandleList::iterator& itr);
	void RevTmxSize(HandleList::iterator& itr);
	void RevTmxData(HandleList::iterator& itr);
	void RevPosData(HandleList::iterator& itr);
	void RevSetBombData(HandleList::iterator& itr);
	void RevDethData(HandleList::iterator& itr);
	void RevResultData(HandleList::iterator& itr);
	void RevLostData(HandleList::iterator& itr);

	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	std::thread update_;
	std::mutex stMtx_;
	std::mutex revMtx_;
	std::mutex handleMtx_;
	std::mutex nowMtx_;
	std::mutex roundMtx_;
	std::mutex resultMtx_;
	std::mutex revIDMtx_;
	std::mutex objRevMtx_;
	// �e�I�u�W�F������M�f�[�^�i�[��ւ̎Q��
	ObjRevMap objRevMap_;

	std::map<MES_TYPE, int> checkData_;

	std::list<int> resultData_;

	NetWork();
	~NetWork();
};

