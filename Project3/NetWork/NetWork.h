#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include <list>
#include <chrono>
#include <DxLib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

#define ONE_SEND_MES 1000

using MesDataVec = std::vector<sendData>;
using IParray = std::array<IPDATA, 5>;

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

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// �w�肳�ꂽIP�A�h���X�̃z�X�g�֐ڑ����ɍs��
	NetWorkMode GetMode(void);
	void SendMes(MesDataVec data);
	void SendStanby(void);
	void SendStart(void);
	bool SendTmxData(std::string filename);

	void SaveTmx(void);

	bool GetRevStanby(void);
	bool GetGameStart(void);

	MES_H PickUpMes(void);

private:
	bool revState_;
	bool gameStart_;
	MesDataVec revData_;
	int revSize_;
	int cntRev_;

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


	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	std::thread update_;
	std::mutex mesMtx_;
	std::mutex stMtx_;

	std::list<MES_H> mesList_;

	NetWork();
	~NetWork();
};

