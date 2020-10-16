#include <iostream>
#include <DxLib.h>
#include "HostState.h"
#include "../_debug/_DebugConOut.h"

HostState::HostState()
{
	// �ڑ���t�J�n���Ă�����0�Ȃ̂ł���Ȋ���
	PreparationListenNetWork(portNum_);
	active_ = ACTIVE_STATE::WAIT;
	std::cout << "�ڑ��ҋ@" << std::endl;
}

HostState::~HostState()
{
}

bool HostState::CheckNetState(void)
{
	auto tmpID = GetNewAcceptNetWork();
	if (tmpID != -1)
	{
		// ����͕����l��z�肵�Ă��Ȃ��̂ň�l�ڑ����Ă�����V�K��t���I������
		netHandle_ = tmpID;
		active_ = ACTIVE_STATE::INIT;
		std::cout << "�ڑ�����܂���" << std::endl;
		StopListenNetWork();
	}

	if (GetLostNetWork()!=-1)
	{
		// �Q�X�g����؂�ꂽ�ꍇ�Đڑ��҂�
		PreparationListenNetWork(portNum_);
		active_ = ACTIVE_STATE::WAIT;
		std::cout << "�ؒf����܂���" << std::endl;
		// �z�X�g���玫�߂����Ƃ��͎蓮��CloseNetWork�Ăڂ��l
		return false;
	}
	return true;
}
