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

#define REV_FUNC_SIZE (static_cast<size_t>(MES_TYPE::MAX) - static_cast<int>(MES_TYPE::NON))

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
	void SetPlayNow(bool flag);

	bool SetActivMode(ACTIVE_STATE state);
	bool SetNetWorkMode(NetWorkMode mode);
	bool SetCountDownRoomTime(const std::chrono::system_clock::time_point& time);
	bool SetCountDownGameTime(const std::chrono::system_clock::time_point& time);
	ACTIVE_STATE GetActive(void);
	IParray GetIP(void);
	bool ConnectHost(IPDATA hostIP);					// 指定されたIPアドレスのホストへ接続しに行く
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
	bool GetRevResult(void);

	void SaveTmx(void);

	void EndOfNetWork(void);


private:
	bool revState_;
	bool startGame_;
	bool playNow_;
	bool revResult_;
	bool revID_;
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

	// ネットワーク関係本体
	std::unique_ptr<NetWorkState> state_;

	// レシーブ部メンバ関数ポインタ 高速化する際arrayにしたが追加がめんどくさい
	using RevFunc = std::function<void(HandleList::iterator& itr)>;
	std::array<RevFunc, REV_FUNC_SIZE > revFunc_;

	// レシーブ部メンバ関数
	// 各関数には担当するMES_TYPEに対するガードを入れる
	void RevNonMes(HandleList::iterator& itr);
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

	// 受信時間計測用 Debugでしか表示してないのでいらないかもしれない
	std::chrono::system_clock::time_point strat_;
	std::chrono::system_clock::time_point end_;

	// 高速化のためのthread 直接これに対して構築するのではなくRunUpdateを呼ぶ
	std::thread update_;
	// mutex類 いい感じの管理方法募集中
	std::mutex stMtx_;
	std::mutex revMtx_;
	std::mutex handleMtx_;
	std::mutex nowMtx_;
	std::mutex resultMtx_;
	std::mutex revIDMtx_;
	std::mutex objRevMtx_;

	// 各オブジェが持つ受信データ格納先への参照
	ObjRevMap objRevMap_;

	// 飛んできたdataに対してそのMES_TYPEで想定されているサイズ分dataが存在しているかの確認用
	std::map<MES_TYPE, int> checkData_;

	// result表示用データ格納先
	std::list<int> resultData_;

	MES_TYPE oldType_;

	NetWork();
	~NetWork();
};

