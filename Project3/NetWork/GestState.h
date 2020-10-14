#pragma once
#include "NetWorkState.h"
class GestState :
	public NetWorkState
{
public:
	GestState();
	~GestState();
	NetWorkMode GetMode(void)override
	{
		return NetWorkMode::GEST;
	}
};

