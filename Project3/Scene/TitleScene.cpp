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
	pos_ = Vector2{};
	speed_ = 5; 
	rad_ = 0;
	input_ = std::make_unique<PadState>();
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
			IPDATA hostIP;
			lpNetWork.SetNetWorkMode(NetWorkMode::GEST);

			std::string ip;
			std::cout << "ホストのIPアドレス(IPv4)を入力してください" << std::endl;
			std::cout << "IPアドレス(IPv4)は<.>で区切ってください" << std::endl;
			std::cin >> ip;

			std::replace(ip.begin(), ip.end(), '.', ' ');
			std::istringstream ipstr(ip);

			std::string ip1, ip2, ip3, ip4;
			ipstr >> ip1 >> ip2 >> ip3 >> ip4;

			hostIP.d1 = atoi(ip1.c_str());
			hostIP.d2 = atoi(ip2.c_str());
			hostIP.d3 = atoi(ip3.c_str());
			hostIP.d4 = atoi(ip4.c_str());

			lpNetWork.ConnectHost(hostIP);
			TRACE("入力されたIPアドレスは%d.%d.%d.%dです\n", hostIP.d1, hostIP.d2, hostIP.d3, hostIP.d4);
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
	TRACE("状態は%dです\n", lpNetWork.GetActive());
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screenSize_X, &screenSize_Y);
	Image = LoadGraph("Image/ばつ.png");
}

unipueBase TitleScene::Update(unipueBase own)
{
	input_->Update();

	auto moveTest = [&](bool flag, Vector2 speed)
	{  
		if (flag)
		{
			pos_ += speed;
		}
	};

	moveTest(input_->GetKeySty(INPUT_ID::LEFT), { -speed_,0 });
	moveTest(input_->GetKeySty(INPUT_ID::RIGHT), { speed_,0 });
	moveTest(input_->GetKeySty(INPUT_ID::UP), { 0,-speed_ });
	moveTest(input_->GetKeySty(INPUT_ID::DOWN), { 0,speed_ });

	POS_DATA mes = { pos_.x,pos_.y };
	if (lpNetWork.UpDate())
	{
		lpNetWork.SendMes(mes);
		lpNetWork.RecvMes(pos_);
	}

	return own;
}

void TitleScene::Draw(void)
{
	DrawGraph(pos_.x, pos_.y, Image, true);
}


