#include <iostream>
#include <DxLib.h>
#include "HostState.h"
#include "NetWork.h"
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
	//auto tmpID = GetNewAcceptNetWork();
	//if (tmpID != -1)
	//{
	//	// ����͕����l��z�肵�Ă��Ȃ��̂ň�l�ڑ����Ă�����V�K��t���I������
	//	netHandleList_ = tmpID;
	//	active_ = ACTIVE_STATE::INIT;
	//	std::cout << "�ڑ�����܂���" << std::endl;
	//	StopListenNetWork();
	//}

	//if (GetLostNetWork() != -1)
	//{
	//	// �Q�X�g����؂�ꂽ�ꍇ�Đڑ��҂�
	//	PreparationListenNetWork(portNum_);
	//	active_ = ACTIVE_STATE::WAIT;
	//	netHandleList_ = -1;
	//	std::cout << "�ؒf����܂���" << std::endl;
	//	auto ipVec = lpNetWork.GetIP();
	//	for (auto ip : ipVec)
	//	{
	//		if (ip.d1 != 0)
	//		{
	//			std::string mes = ip.d1 == 192 ? "�O���[�o��" : "���[�J��";
	//			TRACE("������%sIP�A�h���X��%d.%d.%d.%d�ł�\n", mes.c_str(), ip.d1, ip.d2, ip.d3, ip.d4);
	//		}
	//	}
	//	// �z�X�g���玫�߂����Ƃ��͎蓮��CloseNetWork�Ăڂ��l
	//	return false;
	//}
	return true;
}
