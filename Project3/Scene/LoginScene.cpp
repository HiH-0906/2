#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>
#include "LoginScene.h"
#include "CheckeredBlock.h"
#include "CrossOver.h"
#include "GameScene.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"
#include "../State/INPUT_ID.h"
#include "../_debug/_DebugConOut.h"
#include "../TmxLoader/TmxLoader.h"

// ����Ƃ肠�����o�Ԃ����܂�Ă��銴����
LoginScene::LoginScene()
{
	screenSize_X = 0;
	screenSize_Y = 0;
	drawScreen_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, true);
	pos_ = Vector2{};
	ipData_ = {};
	Image = LoadGraph("Image/game.png", true);
	input_ = std::make_unique<PadState>();
	state_ = UPDATE_STATE::SET_NET;
	func_.try_emplace(UPDATE_STATE::SET_NET, &LoginScene::SetNetWork);
	func_.try_emplace(UPDATE_STATE::HOST_IP, &LoginScene::HostIPInput);
	func_.try_emplace(UPDATE_STATE::START_INIT, &LoginScene::StartInit);
	func_.try_emplace(UPDATE_STATE::SELECT_HOST, &LoginScene::SelectHost);
	func_.try_emplace(UPDATE_STATE::READ_HOST, &LoginScene::ReadHost);
	sendTmx_ = false;
	reset_ = false;
	waitTime_ = {};
	tetHight_ = 400;

	DrawOwnScene();
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init(void)
{
}

uniqueBase LoginScene::Update(uniqueBase own, const Time& now)
{
	DrawOwnScene();
	if (!(this->*func_[state_])())
	{
		return std::move(std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<GameScene>()));
	}
	if (reset_)
	{
		lpNetWork.EndOfNetWork();
		return std::move(std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<LoginScene>()));
	}
	return own;
}

void LoginScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();

	if (sendTmx_)
	{
		auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - waitTime_).count();
		DrawFormatString(300, 500, 0xffffff, "�����҂��F%d�b", cnt / 1000);
		if (cnt >= (ResetTime))
		{
			reset_ = true;
		}
	}
	else
	{
		DrawGraph(0, 0, Image, true);
		if (lpNetWork.GetCountDownFlag())
		{
			auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lpNetWork.GetCountDownRoomTime()).count();
			cnt = INIT_COUNT_TIME - cnt;
			if (cnt <= 0)
			{
				cnt = 0;
			}
			DrawFormatString(300, 500, 0xffffff, "�J�n�܂ł��ƁF%d�b", cnt / 1000);
		}
		else
		{
			DrawFormatString(300, 500, 0xffffff, "�ҋ@��");
		}
	}
	
}

// �Q�X�g��p �z�X�g��IP�A�h���X���͂�����֐� ���͂��ꂽ�z�X�g��IP�A�h���X�Őڑ��ł��Ȃ��ꍇ�ē���
bool LoginScene::HostIPInput(void)
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

	TRACE("���͂��ꂽIP�A�h���X��%d.%d.%d.%d�ł�\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);

	if (!lpNetWork.ConnectHost(ipData_))
	{
		std::cout << "�z�X�g�ɐڑ��ł��܂���ł����B������x���͂��Ă��������B" << std::endl;
		return true;
	}
	
	WritFile();
	state_ = UPDATE_STATE::START_INIT;

	return true;
}

// ���p �l�b�g�g�����ǂ����A�z�X�g���Q�X�g���I��
bool LoginScene::SetNetWork(void)
{
	auto ipVec = lpNetWork.GetIP();
	for (auto& ip : ipVec)
	{
		if (ip.d1 != 0)
		{
			std::string mes = ip.d1 == 192 ? "�O���[�o��" : "���[�J��";
			TRACE("������%sIP�A�h���X��%d.%d.%d.%d�ł�\n", mes.c_str(), ip.d1, ip.d2, ip.d3, ip.d4);
		}
	}
	bool loop = true;
	do
	{
		std::cout << "�L�[����͂��ăl�b�g���[�h���w�肵�Ă�������" << std::endl;
		std::cout << "0:OffLine\n1:Host\n2:Gest\n" << std::endl;
		std::string num = {};
		std::cin >> num;
		if (num == "0")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
			
			std::cout << "�I�t���C���ł�\n" << std::endl;
			loop = false;
			return false;
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

// ���p ���������ă��b�Z�[�W��΂� �z�X�g�A�Q�X�g�����Ă��悩�������Ǒ債���ʂł��Ȃ����A�X�e�[�g�����邵�ŕ����Ȃ��Ă������Ȃ���
bool LoginScene::StartInit(void)
{

	if (lpNetWork.GetMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT)
		{
			if (!sendTmx_)
			{
				sendTmx_ = true;
				lpNetWork.SendTmxData("mapData/map.tmx");
				MesDataList list;
				lpNetWork.SendMesAll(MES_TYPE::STANBY_HOST, list);
				waitTime_ = std::chrono::system_clock::now();
			}
			if (lpNetWork.GetMax() == lpNetWork.GetStanbyPlayerNum())
			{
				unionTimeData time = { std::chrono::system_clock::now() };
				sendData data[2];
				data[0].idata = time.idata[0];
				data[1].idata = time.idata[1];
				lpNetWork.SetCountDownGameTime(time.time);
				lpNetWork.SendMesAll(MES_TYPE::COUNT_DOWN_GAME, MesDataList{ data[0],data[1] });
				lpNetWork.SetActivMode(ACTIVE_STATE::STANBY);
				TRACE("�X�^���o�C����\n");
				return true;
			}
		}
		if (lpNetWork.GetActive() == ACTIVE_STATE::STANBY && lpNetWork.GetStanbyPlayerNum() != 0)
		{
			std::cout << "�J�n" << std::endl;
			lpNetWork.SetStartGame(true);
			return false;
		}
	}

	if (lpNetWork.GetMode() == NetWorkMode::GEST)
	{
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT && lpNetWork.GetRevStanby())
		{
			MesDataList data;
			lpNetWork.SendMes(MES_TYPE::STANBY_GUEST, data);
			std::cout << "�J�n" << std::endl;
			return false;
		}
	}
	return true;
}

// �Q�X�g��p �O����͂����z�X�g�ɐڑ����邩�V���ɓ��͂��邩�̑I��
bool LoginScene::SelectHost(void)
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

// �Q�X�g��p �t�@�C������̃z�X�g�̓ǂݍ��� �t�@�C�����Ȃ������ꍇ�A�ǂݍ���IP�A�h���X�Őڑ��ł��Ȃ������ꍇ���͂ֈڍs
bool LoginScene::ReadHost(void)
{
	if(!ReadFile())
	{
		std::cout << "�t�@�C����ǂݍ��߂܂���ł����B���͂ֈڍs���܂��B" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
		return true;
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
// �t�@�C������̓ǂݍ���
bool LoginScene::ReadFile(void)
{
	std::fstream file("ini/IPData.dat", std::ios::binary | std::ios::in);

	if (!file)
	{
		return false;
	}

	file.read((char*)&ipData_, sizeof(ipData_));
	TRACE("�ǂݍ��܂ꂽIP�A�h���X��%d.%d.%d.%d�ł�\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);
	file.close();
	return true;
}
// �t�@�C���ւ̏�������
bool LoginScene::WritFile(void)
{
	std::fstream file("ini/IPData.dat", std::ios::binary | std::ios::out);

	if(!file)
	{
		return false;
	}

	file.write((char*)&ipData_, sizeof(ipData_));

	file.close();

	return true;
}
