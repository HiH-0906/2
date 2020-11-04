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

// 現状とりあえず出ぶち込まれている感満載
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

// ゲスト専用 ホストのIPアドレス入力させる関数 入力されたホストのIPアドレスで接続できない場合再入力
bool LoginScene::HostIPInput(void)
{
	state_ = UPDATE_STATE::HOST_IP;
	std::string ip;
	std::cout << "ホストのIPアドレス(IPv4)を入力してください" << std::endl;
	std::cout << "IPアドレス(IPv4)は<.>で区切ってください" << std::endl;
	std::cin >> ip;

	std::replace(ip.begin(), ip.end(), '.', ' ');
	std::istringstream ipstr(ip);

	std::string ip1, ip2, ip3, ip4;
	ipstr >> ip1 >> ip2 >> ip3 >> ip4;

	ipData_.d1 = atoi(ip1.c_str());
	ipData_.d2 = atoi(ip2.c_str());
	ipData_.d3 = atoi(ip3.c_str());
	ipData_.d4 = atoi(ip4.c_str());

	TRACE("入力されたIPアドレスは%d.%d.%d.%dです\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);

	if (!lpNetWork.ConnectHost(ipData_))
	{
		std::cout << "ホストに接続できませんでした。もう一度入力してください。" << std::endl;
		return true;
	}
	
	WritFile();
	state_ = UPDATE_STATE::START_INIT;

	return true;
}

// 共用 ゲーム本編
bool LoginScene::PlayUpdate(void)
{
	return false;
}
// 共用 ネット使うかどうか、ホストかゲストか選択
bool LoginScene::SetNetWork(void)
{
	auto ipVec = lpNetWork.GetIP();
	for (auto ip : ipVec)
	{
		if (ip.d1 != 0)
		{
			std::string mes = ip.d1 == 192 ? "グローバル" : "ローカル";
			TRACE("自分の%sIPアドレスは%d.%d.%d.%dです\n", mes.c_str(), ip.d1, ip.d2, ip.d3, ip.d4);
		}
	}
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
			
			std::cout << "オフラインです\n" << std::endl;
			state_ = UPDATE_STATE::PLAY;
			loop = false;
		}
		else if (num == "1")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
			std::cout << "ホストです\n" << std::endl;
			state_ = UPDATE_STATE::START_INIT;
			loop = false;
		}
		else if (num == "2")
		{
			lpNetWork.SetNetWorkMode(NetWorkMode::GEST);
			std::cout << "ゲストです\n" << std::endl;
			state_ = UPDATE_STATE::SELECT_HOST;
			loop = false;
		}
		else
		{
			std::cout << "指定された値を入力してください\n" << std::endl;
		}
	} while (loop);
	return true;
}

// 共用 初期化してメッセージ飛ばす ホスト、ゲスト分けてもよかったけど大した量でもないし、ステート増えるしで分けなくていいかなって
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
			std::cout << "開始" << std::endl;
			state_ = UPDATE_STATE::PLAY;
		}
	}

	if (lpNetWork.GetMode() == NetWorkMode::GEST)
	{
		auto pos = Vector2{};
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT && lpNetWork.GetRevStanby())
		{
			state_ = UPDATE_STATE::PLAY;
			std::cout << "開始" << std::endl;
			lpNetWork.SendStart();
		}
	}
	return true;
}

// ゲスト専用 前回入力したホストに接続するか新たに入力するかの選択
bool LoginScene::SelectHost(void)
{
	int select;
	std::cout << "前回接続したホストに接続しますか？" << std::endl;
	std::cout << "1：前回接続したホストに接続する" << std::endl;
	std::cout << "2：新たにIPを入力する" << std::endl;
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
		std::cout << "1または2を入力してください" << std::endl;
	}
	return true;
}

// ゲスト専用 ファイルからのホストの読み込み ファイルがなかった場合、読み込んだIPアドレスで接続できなかった場合入力へ移行
bool LoginScene::ReadHost(void)
{
	if(!ReadFile())
	{
		std::cout << "ファイルを読み込めませんでした。入力へ移行します。" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
		return true;
	}
	if (lpNetWork.ConnectHost(ipData_))
	{
		std::cout << "接続完了。" << std::endl;
		state_ = UPDATE_STATE::START_INIT;
	}
	else
	{
		std::cout << "前回ホストへ接続できませんでした。入力へ移行します。" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
	}
	return true;
}
// ファイルからの読み込み
bool LoginScene::ReadFile(void)
{
	std::fstream file("Data/IPData.dat", std::ios::binary | std::ios::in);

	if (!file)
	{
		return false;
	}

	file.read((char*)&ipData_, sizeof(ipData_));
	TRACE("読み込まれたIPアドレスは%d.%d.%d.%dです\n", ipData_.d1, ipData_.d2, ipData_.d3, ipData_.d4);
	file.close();
	return true;
}
// ファイルへの書き込み
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
	std::ifstream tmxstr(filename.c_str());
	tmxstr.seekg(0, std::ios::end);
	MES_H data = { MES_TYPE::TMX_SIZE,0,0,sizeof(TMX_SIZE) };
	lpNetWork.SendMes(data);
	return true;
}
