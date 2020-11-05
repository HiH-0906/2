#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>
#include "LoginScene.h"
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
	func_.try_emplace(UPDATE_STATE::PLAY, &LoginScene::PlayUpdate);
	func_.try_emplace(UPDATE_STATE::SELECT_HOST, &LoginScene::SelectHost);
	func_.try_emplace(UPDATE_STATE::READ_HOST, &LoginScene::ReadHost);

	DrawOwnScene();
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init(void)
{
}

uniqueBase LoginScene::Update(uniqueBase own)
{
	DrawOwnScene();
	if (!(this->*func_[state_])())
	{
		return std::move(std::make_unique<CrossOver>(std::move(own), std::make_unique<GameScene>()));
	}
	return own;
}

void LoginScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	DrawGraph(0, 0, Image, true);
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

// ���p �Q�[���{��
bool LoginScene::PlayUpdate(void)
{
	return false;
}
// ���p �l�b�g�g�����ǂ����A�z�X�g���Q�X�g���I��
bool LoginScene::SetNetWork(void)
{
	auto ipVec = lpNetWork.GetIP();
	for (auto ip : ipVec)
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

// ���p ���������ă��b�Z�[�W��΂� �z�X�g�A�Q�X�g�����Ă��悩�������Ǒ債���ʂł��Ȃ����A�X�e�[�g�����邵�ŕ����Ȃ��Ă������Ȃ���
bool LoginScene::StartInit(void)
{

	if (lpNetWork.GetMode() == NetWorkMode::HOST)
	{
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT)
		{
			SendNetWorkMes("mapData/map.tmx");
			if (lpNetWork.SendTmxData("mapData/map.tmx"))
			{
				lpNetWork.SendStanby();
			}
		}
		if (lpNetWork.GetActive() == ACTIVE_STATE::STANBY && lpNetWork.GetGameStart())
		{
			std::cout << "�J�n" << std::endl;
			state_ = UPDATE_STATE::PLAY;
		}
	}

	if (lpNetWork.GetMode() == NetWorkMode::GEST)
	{
		auto pos = Vector2{};
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT && lpNetWork.GetRevStanby())
		{
			state_ = UPDATE_STATE::PLAY;
			std::cout << "�J�n" << std::endl;
			lpNetWork.SendStart();
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
// �t�@�C���ւ̏�������
bool LoginScene::WritFile(void)
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

bool LoginScene::SendNetWorkMes(std::string filename)
{
	MesDataVec Mes;
	std::ifstream tmxstr(filename.c_str());
	tmxstr.seekg(0, std::ios::end);
	mes_H data;
	data.head = { MES_TYPE::TMX_SIZE,0,0,sizeof(TMX_SIZE) };
	auto hsize = sizeof(MES_H) / sizeof(sendData);
	auto dsize = sizeof(TMX_SIZE) / sizeof(sendData);
	Mes.resize(hsize + dsize);
	Mes[0].idata = data.ihead[0];
	Mes[1].idata = data.ihead[1];
	TMX_SIZE sizedata = { (tmxstr.tellg() / ONE_SEND_MES),ONE_SEND_MES,tmxstr.tellg() };
	Mes[2].idata = sizedata.num;
	Mes[3].idata = sizedata.size;
	Mes[4].idata = sizedata.allsize;
	lpNetWork.SendMes(Mes);
	return true;
}
