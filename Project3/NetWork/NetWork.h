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

#define REv_FUNC_SIZE (static_cast<size_t>(MES_TYPE::MAX) - static_cast<int>(MES_TYPE::NON))

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

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// �w�肳�ꂽIP�A�h���X�̃z�X�g�֐ڑ����ɍs��
	NetWorkMode GetMode(void);
	void SendMesAll(MES_TYPE type, MesDataList data);
	void SendMes(MES_TYPE type, MesDataList data);
	void SendMes(MES_TYPE type, MesDataList data,int handle);
	void SendMes(MES_TYPE type,int handle);
	bool SendTmxData(std::string filename);

	void SaveTmx(void);

	void EndOfNetWork(void);

	bool GetRevStanby(void);
	bool GetGameStart(void);
private:
	bool revState_;
	bool gameStart_;
	bool netEnd_;
	MesDataList revDataList_;
	MES_H mes_;
	int revSize_;
	int cntRev_;
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
	using RevFunc = std::function<void(void)>;
	std::array<RevFunc, REv_FUNC_SIZE > revFunc_;

	void RevCountDown(void);
	void RevID(void);
	void RevStanby(void);
	void RevStartTime(void);
	void RevGameStart(void);
	void RevTmxSize(void);
	void RevTmxData(void);
	void RevData(void);

	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	std::thread update_;
	std::mutex stMtx_;
	std::mutex revMtx_;
	// �e�I�u�W�F������M�f�[�^�i�[��ւ̎Q��
	ObjRevMap objRevMap_;

	NetWork();
	~NetWork();
};

