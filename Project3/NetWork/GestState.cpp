#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
	active_ = false;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	// �z�X�g�֐ڑ��ɍs�� 0�ȏ�Ȃ�ڑ��m��
	netHandle_ = ConnectNetWork(hostIP, portNum_);
	if (netHandle_ >= 0)
	{
		return true;
	}
	return false;
}
