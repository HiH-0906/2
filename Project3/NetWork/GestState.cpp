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
		active_ = true;
		return true;
	}
	return false;
}

void GestState::SendMes(POS_DATA data)
{
	NetWorkSend(netHandle_, &data, sizeof(POS_DATA));
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
