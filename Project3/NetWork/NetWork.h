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

#define REv_FUNC_SIZE (static_cast<size_t>(MES_TYPE::MAX) - static_cast<int>(MES_TYPE::NON))

#define BIT_NUM 8

using MesDataList = std::vector<sendData>;
using IParray = std::array<IPDATA, 5>;
using RevDataP = std::pair<MES_H, MesDataList>;
using ObjRevMap = std::vector< std::pair<std::mutex&, std::vector<RevDataP>&>>;

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

	void SetObjRevData(int id, std::mutex& mtx, std::vector<RevDataP>& mes);

	bool SetNetWorkMode(NetWorkMode mode);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// 指定されたIPアドレスのホストへ接続しに行く
	NetWorkMode GetMode(void);
	void SendMesAll(MES_TYPE type, MesDataList data);
	void SendMesAll(MES_TYPE type, MesDataList data,int noSendHandle);
	void SendMes(MES_TYPE type, MesDataList data);
	void SendMes(MES_TYPE type, MesDataList data,int handle);
	void SendMes(MES_TYPE type,int handle);
	bool SendTmxData(std::string filename);

	bool GetCountDownFlag(void);
	const std::chrono::system_clock::time_point& GetCountDownTime(void)const;

	const int GetID(void)const;
	const int GetMax(void)const;
	const int& GetRevCount(void)const;

	void SaveTmx(void);

	void EndOfNetWork(void);

	bool GetRevStanby(void);
	bool GetGameStart(void);
private:
	bool revState_;
	bool gameStart_;
	MesDataList revDataList_;
	MES_H mes_;
	int revSize_;
	int cntRev_;
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

	// ネットワーク関係本体
	std::unique_ptr<NetWorkState> state_;

	// レシーブ部メンバ関数ポインタ
	using RevFunc = std::function<void(void)>;
	std::array<RevFunc, REv_FUNC_SIZE > revFunc_;

	void RevCountDownRoom(void);
	void RevID(void);
	void RevStanbyHost(void);
	void RevCountDownGame(void);
	void RevStanbyGuest(void);
	void RevTmxSize(void);
	void RevTmxData(void);
	void RevData(void);

	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	std::thread update_;
	std::mutex stMtx_;
	std::mutex revMtx_;
	// 各オブジェが持つ受信データ格納先への参照
	ObjRevMap objRevMap_;

	NetWork();
	~NetWork();
};

