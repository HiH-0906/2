#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include <chrono>
#include "../common/Vector2.h"

#define INIT_COUNT_TIME 15000

// ネット状態判断用
enum class NetWorkMode
{
	NON,
	OFFLINE,
	HOST,
	GEST,
	MAX
};

enum class ACTIVE_STATE
{
	NON,
	WAIT,
	INIT,
	STANBY,
	PLAY,
	OFFLINE
};

enum class MES_TYPE :unsigned char
{
	NON = 100,
	COUNT_DOWN_ROOM,
	ID,
	STANBY_HOST,
	STANBY_GUEST,
	COUNT_DOWN_GAME,
	TMX_SIZE,
	TMX_DATA,
	POS,
	SET_BOMB,
	DETH,
	LOST,
	MAX
};


struct MES_H
{
	MES_TYPE type;
	unsigned char next;
	unsigned short sendID;
	unsigned int length;
};

struct TMX_SIZE
{
	unsigned int size;
};

struct IP_DATA
{
	int handle;
	unsigned int id;
	int state;
};

union mes_H
{
	MES_H head;
	unsigned int ihead[2];
};

union sendData
{
	unsigned int uidata;
	int idata;
	unsigned char cdata[4];
};

union unionTimeData
{
	std::chrono::system_clock::time_point time;
	unsigned int idata[2];
};
// 構造体ﾊﾝﾄﾞﾙIdSTATE-1切断0ニュートラル1完了
using HandleList = std::list<IP_DATA>;

// ネット接続モジュールの基盤 OFFLINE時はこれがインスタンスされる
class NetWorkState
{
public:
	NetWorkState();
	bool Update(void);
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void)
	{
		return NetWorkMode::OFFLINE;
	}

	void SetCountTime(std::chrono::system_clock::time_point time);
	void SetPlayerID(int id, unsigned int max);
	ACTIVE_STATE GetActive(void);
	const HandleList& GetNetHandle(void)const;
	bool GetCountStart(void);
	bool GetGameStart(void);
	const std::chrono::system_clock::time_point& GetCountDownTime(void)const;
	virtual bool ConnectHost(IPDATA hostIP);						// ホストやオフラインの時は必ずfalse
	bool SetActive(ACTIVE_STATE state);
	const int& GetID(void)const;
	const int& GetMax(void)const;
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// 接続時ポート番号 数字は先生のお気に入りの番号
	HandleList netHandleList_;
	ACTIVE_STATE active_;											// 接続開始しているかどうか
	std::chrono::system_clock::time_point countTime_;
	bool countDown_;
	bool gameStart_;
	int playerID_;
	int playerMax_;
};

