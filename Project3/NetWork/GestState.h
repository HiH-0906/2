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
	bool ConnectHost(IPDATA hostIP)override final;
	void SendMes(POS_DATA data)override;
private:
	bool CheckNetState(void)override;
};

