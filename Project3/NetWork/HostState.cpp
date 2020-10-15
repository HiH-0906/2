#include "DxLib.h"
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// �ڑ���t�J�n���Ă�����0�Ȃ̂ł���Ȋ���
	active_ = PreparationListenNetWork(portNum_) == 0 ? true : false;
}

HostState::~HostState()
{
}

void HostState::RecvMes(Vector2& pos)
{
	if (GetNetWorkDataLength(netHandle_) >= sizeof(POS_DATA))
	{
		POS_DATA data;
		NetWorkRecv(netHandle_, &data, sizeof(POS_DATA));
		pos = { data.x,data.y };
	}
}

bool HostState::CheckNetState(void)
{
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		// ����͕����l��z�肵�Ă��Ȃ��̂ň�l�ڑ����Ă�����V�K��t���I������
		netHandle_ = tmpID;
		StopListenNetWork();
	}

	if (GetLostNetWork()!=-1)
	{
		// �Q�X�g����؂�ꂽ�ꍇ�Đڑ��҂�
		PreparationListenNetWork(portNum_);
		// �z�X�g���玫�߂����Ƃ��͎蓮��CloseNetWork�Ăڂ��l
		return false;
	}
	return true;
}
