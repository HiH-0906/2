#pragma once
#include <DxLib.h>
#include <map>
#include <functional>
#include "../common/Vector2.h"

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

enum class MES_TYPE:unsigned char
{
	STANBY,
	GAME_START,
	TMX_SIZE,
	TMX_DATA,
	POS
};


struct MES_DATA
{
	MES_TYPE type;
	unsigned short id;
	unsigned char cdata;
	int data[2];
};

union sendData
{
	unsigned char cdata[8];
	unsigned int idata[2];
	long long ldata;
};


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

	ACTIVE_STATE GetActive(void);
	int GetNetHandle(void);
	virtual bool ConnectHost(IPDATA hostIP);						// ホストやオフラインの時は必ずfalse
	bool SetActive(ACTIVE_STATE state);
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// 接続時ポート番号 数字は先生のお気に入りの番号
	int netHandle_;
	ACTIVE_STATE active_;											// 接続開始しているかどうか
};

