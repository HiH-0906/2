#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>
#include "TitleScene.h"
#include "../NetWork/NetWork.h"
#include "../State/INPUT_ID.h"
#include "../_debug/_DebugConOut.h"
#include "../TmxLoad/TmxLoadr.h"


TitleScene::TitleScene()
{
	screenSize_X = 0;
	screenSize_Y = 0;
	pos_ = Vector2{};
	speed_ = 5; 
	rad_ = 0;
	ipData_ = {};
	input_ = std::make_unique<PadState>();
	state_ = UPDATE_STATE::SET_NET;
	func_.try_emplace(UPDATE_STATE::SET_NET, &TitleScene::SetNetWork);
	func_.try_emplace(UPDATE_STATE::HOST_IP, &TitleScene::HostIPInput);
	func_.try_emplace(UPDATE_STATE::START_INIT, &TitleScene::StartInit);
	func_.try_emplace(UPDATE_STATE::PLAY, &TitleScene::PlayUpdate);
	func_.try_emplace(UPDATE_STATE::SELECT_HOST, &TitleScene::SelectHost);
	func_.try_emplace(UPDATE_STATE::READ_HOST, &TitleScene::ReadHost);

	mapMng_ = std::make_unique<Map>();
	
	mapMng_->LoadMap();


	LoadDivGraph("Image/map.png", 12, 4, 3, 32, 32, &image_[0], true);
}

TitleScene::~TitleScene()
{
}

void TitleScene::Init(void)
{
	GetDrawScreenSize(&screenSize_X, &screenSize_Y);
	Image = LoadGraph("Image/�΂�.png");
}

unipueBase TitleScene::Update(unipueBase own)
{
	if (!lpNetWork.UpDate() && lpNetWork.GetMode() != NetWorkMode::OFFLINE)
	{
		state_ = UPDATE_STATE::START_INIT;
	}
	(this->*func_[state_])();
	return own;
}

void TitleScene::Draw(void)
{
	for (int i = 0; i <= static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, mapMng_->GetDarwMap(static_cast<MapLayer>(i)), true);
	}
}

bool TitleScene::HostIPInput(void)
{
	state_ = UPDATE_STATE::HOST_IP;
	std::string ip;
	std::cout << "�z�X�g��IP�A�h���X(IPv4)����͂��Ă�������" << std::endl;
	std::cout << "IP�A�h���X(IPv4)��<.>�ŋ�؂��Ă�������" << std::endl;
	std::cin >> ip;

	std::replace(ip.begin(), ip.end(), '.', ' ');
	std::istringstream ipstr(ip);

	std::string ip1, ip2, ip3, ip4;
	ipstr >> ip1 >> ip2 >> ip3 >> ip4;

	ipData_.d1 = atoi(ip1.c_str());
	ipData_.d2 = atoi(ip2.c_str());
	ipData_.d3 = atoi(ip3.c_str());
	ipData_.d4 = atoi(ip4.c_str());

	lpNetWork.ConnectHost(ipData_);
	TRACE("���͂��ꂽIP�A�h���X��%d.%d.%d.%d�ł�\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);
	WritFile();
	state_ = UPDATE_STATE::START_INIT;

	return true;
}

bool TitleScene::PlayUpdate(void)
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

	MES_DATA mes = { MES_TYPE::POS,{pos_.x,pos_.y} };

	lpNetWork.SendMes(mes);
	lpNetWork.RecvMes(pos_);

	return false;
}

bool TitleScene::SetNetWork(void)
{
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
			
			std::cout << "�I�t���C���ł�\n" << std::endl;
			state_ = UPDATE_STATE::PLAY;
			loop = false;
		}
		else if (num == "1")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
			std::cout << "�z�X�g�ł�\n" << std::endl;
			state_ = UPDATE_STATE::START_INIT;
			loop = false;
		}
		else if (num == "2")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::GEST);
			std::cout << "�Q�X�g�ł�\n" << std::endl;
			state_ = UPDATE_STATE::SELECT_HOST;
			loop = false;
		}
		else
		{
			std::cout << "�w�肳�ꂽ�l����͂��Ă�������\n" << std::endl;
		}
	} while (loop);
	return true;
}

bool TitleScene::StartInit(void)
{
	if (lpNetWork.GetMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT)
		{
			lpNetWork.SendStanby();
		}
		if (lpNetWork.GetActive() == ACTIVE_STATE::STANBY && lpNetWork.GetRevMesType(MES_TYPE::GAME_START))
		{
			std::cout << "�J�n" << std::endl;
			state_ = UPDATE_STATE::PLAY;
		}
	}

	if (lpNetWork.GetMode() == NetWorkMode::GEST)
	{
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT && lpNetWork.GetRevStanby())
		{
			state_ = UPDATE_STATE::PLAY;
			std::cout << "�J�n" << std::endl;
			lpNetWork.SendStart();
		}
	}
	return false;
}

bool TitleScene::SelectHost(void)
{
	int select;
	std::cout << "�O��ڑ������z�X�g�ɐڑ����܂����H" << std::endl;
	std::cout << "1�F�O��ڑ������z�X�g�ɐڑ�����" << std::endl;
	std::cout << "2�F�V����IP����͂���" << std::endl;
	std::cin >> select;
	if (select == 1)
	{
		state_ = UPDATE_STATE::READ_HOST;
	}
	else if (select == 2)
	{
		state_ = UPDATE_STATE::HOST_IP;
	}
	else
	{
		std::cout << "1�܂���2����͂��Ă�������" << std::endl;
	}
	return true;
}

bool TitleScene::ReadHost(void)
{
	if(!ReadFile())
	{
		std::cout << "�t�@�C����ǂݍ��߂܂���ł����B���͂ֈڍs���܂��B" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
		return false;
	}
	if (lpNetWork.ConnectHost(ipData_))
	{
		std::cout << "�ڑ������B" << std::endl;
		state_ = UPDATE_STATE::START_INIT;
	}
	else
	{
		std::cout << "�O��z�X�g�֐ڑ��ł��܂���ł����B���͂ֈڍs���܂��B" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
	}
	return true;
}

bool TitleScene::ReadFile(void)
{
	std::fstream file("Data/IPData.dat", std::ios::binary | std::ios::in);

	if (!file)
	{
		return false;
	}

	file.read((char*)&ipData_, sizeof(ipData_));
	TRACE("�ǂݍ��܂ꂽIP�A�h���X��%d.%d.%d.%d�ł�\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);
	file.close();
	return true;
}

bool TitleScene::WritFile(void)
{
	std::fstream file("Data/IPData.dat", std::ios::binary | std::ios::out);

	if(!file)
	{
		return false;
	}

	file.write((char*)&ipData_, sizeof(ipData_));

	file.close();

	return true;
}


