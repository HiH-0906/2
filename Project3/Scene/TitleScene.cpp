#include <iostream>
#include <DxLib.h>
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"

TitleScene::TitleScene()
{
	screenSize_X = 0;
	screenSize_Y = 0;
	auto ip = lpNetWork.GetIP();
	TRACE("自分のIPアドレスは%d.%d.%d.%dです\n", ip.d1, ip.d2, ip.d3, ip.d4);
	bool loop = true;
	do
	{
		std::cout << "キーを入力してネットモードを指定してください" << std::endl;
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
			lpNetWork.SetNetWorkMode(NetWorkMode::GEST);
			loop = false;
		}
		else
		{
			std::cout << "指定された値を入力してください\n" << std::endl;
		}
	} while (loop);

	switch (lpNetWork.GetMode())
	{
	case NetWorkMode::OFFLINE:
		std::cout << "オフラインです\n" << std::endl;
		break;
	case NetWorkMode::HOST:
		std::cout << "ホストです\n" << std::endl;
		break;
	case NetWorkMode::GEST:
		std::cout << "ゲストです\n" << std::endl;
		break;
	case NetWorkMode::MAX:
		TRACE("設定されていません｜返り値異常");
		break;
	default:
		TRACE("返り値異常");
		break;
	}
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


