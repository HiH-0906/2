#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <vector>
#include <array>
#include <chrono>
#include <DxLib.h>
#include "NetWorkState.h"

#define lpNetWork NetWork::GetInstance()

using revTmx = std::vector<sendData>;
using IParray = std::array<IPDATA, 5>;

// �����̃V���O���g���N���X
class NetWork
{
public:
	static NetWork& GetInstance()
	{
		return *s_Instance;
	}

	bool UpDate(void);

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// �w�肳�ꂽIP�A�h���X�̃z�X�g�֐ڑ����ɍs��
	NetWorkMode GetMode(void);
	void RecvMes(Vector2& pos);
	void SendMes(MES_DATA data);
	void SendStanby(void);
	void SendStart(void);
	bool SendTmxData(std::string filename);

	bool GetRevMesType(MES_TYPE type);
	bool GetRevStanby(void);

private:
	bool revState_;
	revTmx revTmx_;
	int revSize_;
	int cntRev_;

	IParray ipdata_;

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

	std::thread updatae_;
	std::mutex mesMtx_;
	std::mutex stMtx_;

	NetWork();
	~NetWork();
};

