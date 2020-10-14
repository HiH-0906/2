#include <iostream>
#include <sstream>
#include <DxLib.h>
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	screenSize_X = 0;
	screenSize_Y = 0;
	auto ip = lpNetWork.GetIP();
	TRACE("������IP�A�h���X��%d.%d.%d.%d�ł�\n", ip.d1, ip.d2, ip.d3, ip.d4);
	bool loop = true;
	do
	{
		std::cout << "�L�[����͂��ăl�b�g���[�h���w�肵�Ă�������" << std::endl;
		std::cout << "0:OffLine\n1:Host\n2:Gest\n" << std::endl;
		std::string num;
		std::cin >> num;
		if (num == "0")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
			loop = false;
		}
		else if (num == "1")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
			loop = false;
		}
		else if (num == "2")
		{
			IPDATA hostIP;
			lpNetWork.SetNetWorkMode(NetWorkMode::GEST);
			std::string ip;
			std::cout << "�z�X�g��IP�A�h���X(IPv4)����͂��Ă�������" << std::endl;
			std::cout << "IP�A�h���X(IPv4)��<.>�ŋ�؂��Ă�������" << std::endl;
			std::cin >> ip;
			std::replace(ip.begin(), ip.end(), '.', ' ');
			std::istringstream iss(ip);
			std::string ip1, ip2, ip3, ip4;
			iss >> ip1 >> ip2 >> ip3 >> ip4;
			hostIP.d1 = atoi(ip1.c_str());
			hostIP.d2 = atoi(ip2.c_str());
			hostIP.d3 = atoi(ip3.c_str());
			hostIP.d4 = atoi(ip4.c_str());
			lpNetWork.ConnectHost(hostIP);
			TRACE("���͂��ꂽIP�A�h���X��%d.%d.%d.%d�ł�\n", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);
			loop = false;
		}
		else
		{
			std::cout << "�w�肳�ꂽ�l����͂��Ă�������\n" << std::endl;
		}
	} while (loop);

	switch (lpNetWork.GetMode())
	{
	case NetWorkMode::OFFLINE:
		std::cout << "�I�t���C���ł�\n" << std::endl;
		break;
	case NetWorkMode::HOST:
		std::cout << "�z�X�g�ł�\n" << std::endl;
		break;
	case NetWorkMode::GEST:
		std::cout << "�Q�X�g�ł�\n" << std::endl;
		break;
	case NetWorkMode::MAX:
		TRACE("�ݒ肳��Ă��܂���b�Ԃ�l�ُ�");
		break;
	default:
		TRACE("�Ԃ�l�ُ�");
		break;
	}
	TRACE("��Ԃ�%d�ł�\n", lpNetWork.GetActive());
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screenSize_X, &screenSize_Y);
}

unipueBase TitleScene::Update(unipueBase own)
{
	return own;
}

void TitleScene::Draw(void)
{
}


