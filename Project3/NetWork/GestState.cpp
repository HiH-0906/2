#include <iostream>
#include "GestState.h"
#include "../_debug/_DebugConOut.h"

GestState::GestState()
{
	active_ = ACTIVE_STATE::NON;
}

GestState::~GestState()
{
}

bool GestState::ConnectHost(IPDATA hostIP)
{
	// �z�X�g�֐ڑ��ɍs�� 0�ȏ�Ȃ�ڑ��m��
	netHandleList_ = ConnectNetWork(hostIP, portNum_);
	if (netHandleList_ >= 0)
	{
		active_ = ACTIVE_STATE::INIT;
		std::cout << "������������" << std::endl;
		return true;
	}
	return false;
}

bool GestState::CheckNetState(void)
{
	if (GetLostNetWork() != -1)
	{
		// �z�X�g�ɐؒf���ꂽ�炱����������
		CloseNetWork();
		return false;
	}
	return true;
}
