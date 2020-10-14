#pragma once
#include <DxLib.h>

// ネット状態判断用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GEST,
	MAX
};

// ネット接続モジュールの基盤 OFFLINE時はこれがインスタンスされる
class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void)
	{
		return NetWorkMode::OFFLINE;
	}

	bool GetActive(void);
	virtual bool ConnectHost(IPDATA hostIP);						// ホストやオフラインの時は必ずfalse
private:

protected:
	const unsigned int portNum_ = 8086;								// 接続時ポート番号 数字は先生のお気に入りの番号
	int netHandle_;
	bool active_;													// 接続開始しているかどうか
};

