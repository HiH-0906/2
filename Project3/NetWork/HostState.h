#pragma once
#include "NetWorkState.h"

class HostState :
	public NetWorkState
{
public:
	HostState();
	~HostState();
	NetWorkMode GetMode(void)override
	{
		return NetWorkMode::HOST;
	}
	void RecvMes(Vector2& pos)override;
private:
	bool CheckNetState(void)override;
};

