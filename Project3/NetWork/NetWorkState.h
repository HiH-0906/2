#pragma once

// ネット状態判断用
enum class NetWorkMode
{
	OFFLINE,
	HOST,
	GEST,
	MAX
};

class NetWorkState
{
public:
	NetWorkState();
	virtual ~NetWorkState();
	virtual NetWorkMode GetMode(void)
	{
		return NetWorkMode::OFFLINE;
	}
};

