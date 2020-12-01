#pragma once
#include <chrono>
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
private:
	bool CheckNetState(void)override;
	std::chrono::system_clock::time_point startTime_;
};

