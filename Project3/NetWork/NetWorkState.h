#pragma once
#include <DxLib.h>
#include <functional>
#include "../common/Vector2.h"

// ネット状態判断用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GEST,
	MAX
};

enum class NET_FUNC
{
	CHOICE,
	GUEST_IP,
	HOST_IP,
	ACTIVE,
	CLOSE,
};

struct POS_DATA
{
	int x, y;
};
using NetFunc = std::function<bool(void)>;

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

	virtual void SendMes(POS_DATA data);
	virtual void RecvMes(Vector2& pos);
	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);						// ホストやオフラインの時は必ずfalse
private:
	virtual bool CheckNetState(void) { return false; };
protected:
	void CloseNetWork(void);
	const int portNum_ = 8086;										// 接続時ポート番号 数字は先生のお気に入りの番号
	int netHandle_;
	bool active_;													// 接続開始しているかどうか
};

