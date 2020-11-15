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
/// データ受け取り時の定数
/// </summary>
#define CSV_HEIGHT 17
#define CSV_WIDTH 21
#define CSV_SIZE (CSV_HEIGHT*CSV_WIDTH)
#define CSV_LAYER 4
#define ALL_CSV_SIZE (CSV_SIZE*CSV_LAYER)

#define BIT_NUM 8

using MesDataList = std::vector<sendData>;
using IParray = std::array<IPDATA, 5>;
using RevData = std::pair<MES_H, MesDataList>;
using ObjRevMap = std::vector< std::pair<std::mutex&, std::vector<RevData>&>>;

// いつものシングルトンクラス
class NetWork
{
public:
	static NetWork& GetInstance()
	{
		return *s_Instance;
	}

	void UpDate(void);
	void RunUpdate(void);

	void SetObjRevData(int id, std::mutex& mtx, std::vector<RevData>& mes);

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// 指定されたIPアドレスのホストへ接続しに行く
	NetWorkMode GetMode(void);
	void SendMes(MES_TYPE type, MesDataList data);
	void SendMes(MES_TYPE type);
	void SendStanby(void);
	void SendStart(void);
	bool SendTmxData(std::string filename);

	void SaveTmx(void);

	bool GetRevStanby(void);
	bool GetGameStart(void);
private:
	bool revState_;
	bool gameStart_;
	MesDataList revData_;
	int revSize_;
	int cntRev_;

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

	// ネットワーク関係本体
	std::unique_ptr<NetWorkState> state_;


	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	std::thread update_;
	std::mutex mesMtx_;
	std::mutex stMtx_;

	ObjRevMap objRevMap_;

	NetWork();
	~NetWork();
};

