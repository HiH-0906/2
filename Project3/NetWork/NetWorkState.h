#pragma once

// �l�b�g��Ԕ��f�p
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

