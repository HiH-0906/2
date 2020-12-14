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
#include "../Input/PadState.h"
#include "../Input/keyState.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../TmxLoader/TmxLoader.h"

namespace
{
	bool old = false;
	bool now = false;
}

// 現状とりあえず出ぶち込まれている感満載
LoginScene::LoginScene()
{
	Init();
	DrawOwnScene();
}

LoginScene::~LoginScene()
{
}

void LoginScene::Init(void)
{
	screenSize_X = lpSceneMng.GetScreenSize().x;
	screenSize_Y = lpSceneMng.GetScreenSize().y;
	drawScreen_ = MakeScreen(lpSceneMng.GetScreenSize().x, lpSceneMng.GetScreenSize().y, true);
	pos_ = Vector2{};
	ipData_ = {};
	lpImageMng.GetID("LBG", "Image/game.png");
	lpImageMng.GetID("Num", "Image/Number.png", { 33,49 }, { 5,3 });
	lpImageMng.GetID("Calcu", "Image/calcu.png", { 33,49 }, { 5,3 });
	input_ = std::make_unique<keyState>();
	state_ = UPDATE_STATE::SET_NET;
	FuncInit();
	sendTmx_ = false;
	reset_ = false;
	wait_ = false;
	waitTime_ = {};
	ImageInit();
	ButtonInit();
}

uniqueBase LoginScene::Update(uniqueBase own, const Time& now)
{
	if (!(this->*func_[state_])())
	{
		return std::move(std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<GameScene>()));
	}
	if (reset_)
	{
		lpNetWork.EndOfNetWork();
		return std::move(std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<LoginScene>()));
	}
	DrawOwnScene();
	return own;
}

void LoginScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();

	std::sort(DrawQue_.begin(), DrawQue_.end(), [](DrawQue& queA, DrawQue& queB) {return queA.zOder < queB.zOder; });

	for (auto& que : DrawQue_)
	{
		DrawRotaGraph(que.pos.x + (que.size.x / 2), que.pos.y + (que.size.y / 2), que.extRate, que.angle, que.handle, true);
	}
	if (wait_)
	{
		auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - waitTime_).count();
		DrawFormatString(300, 500, 0xffffff, "応答待ち：%d秒", cnt / 1000);
		if (cnt >= (ResetTime))
		{
			reset_ = true;
		}
	}
	else
	{
		if (lpNetWork.GetCountDownFlag())
		{
			auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lpNetWork.GetCountDownRoomTime()).count();
			cnt = INIT_COUNT_TIME - cnt;
			if (cnt <= 0)
			{
				wait_ = true;
				waitTime_ = std::chrono::system_clock::now();
				cnt = 0;
			}
			DrawFormatString(300, 500, 0xffffff, "開始まであと：%d秒", cnt / 1000);
		}
		else
		{
			DrawFormatString(300, 500, 0xffffff, "待機中");
		}
	}
	DrawQue_.clear();
}

void LoginScene::FuncInit(void)
{
	func_.try_emplace(UPDATE_STATE::SET_NET, &LoginScene::SetNetWork);
	func_.try_emplace(UPDATE_STATE::HOST_IP, &LoginScene::HostIPInput);
	func_.try_emplace(UPDATE_STATE::START_INIT, &LoginScene::StartInit);
	func_.try_emplace(UPDATE_STATE::SELECT_HOST, &LoginScene::SelectHost);
	func_.try_emplace(UPDATE_STATE::READ_HOST, &LoginScene::ReadHost);
	func_.try_emplace(UPDATE_STATE::SELECT_INIT, &LoginScene::SelectInit);
}

void LoginScene::ButtonInit(void)
{
	btn_.emplace_back(
		std::make_unique<Button>(Rect{ 268,100,264,90 },
			[&]() {
					lpNetWork.SetNetWorkMode(NetWorkMode::HOST);
					state_ = UPDATE_STATE::START_INIT;
					return true;
				},
			BTN_TYPE::HOST, *this)
	);
	btn_.emplace_back(
		std::make_unique<Button>(Rect{ 268,200,264,90 },
			[&]() {
					lpNetWork.SetNetWorkMode(NetWorkMode::GEST);
					state_ = UPDATE_STATE::SELECT_INIT;
					return true;
				},
			BTN_TYPE::GUEST, *this)
	);
	btn_.emplace_back(
		std::make_unique<Button>(Rect{ 268,300,264,90 },
			[&]() {
						lpNetWork.SetNetWorkMode(NetWorkMode::OFFLINE);
						return false;
				},
			BTN_TYPE::OFFLINE, *this)
	);
}

void LoginScene::ImageInit(void)
{
	lpImageMng.GetID("IPBG", "Image/IPBG.png");

	lpImageMng.GetID("HBtn", "Image/btn/Hbtn.png");
	lpImageMng.GetID("HBtnR", "Image/btn/Hbtn_ride.png");
	lpImageMng.GetID("HBtnD", "Image/btn/Hbtn_push.png");

	lpImageMng.GetID("GBtn", "Image/btn/Gbtn.png");
	lpImageMng.GetID("GBtnR", "Image/btn/Gbtn_ride.png");
	lpImageMng.GetID("GBtnD", "Image/btn/Gbtn_push.png");

	lpImageMng.GetID("OBtn", "Image/btn/Obtn.png");
	lpImageMng.GetID("OBtnR", "Image/btn/Obtn_ride.png");
	lpImageMng.GetID("OBtnD", "Image/btn/Obtn_push.png");
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

bool LoginScene::SelectInit(void)
{
	btn_.clear();
	numPad_ = std::make_unique<NumPad>(Vector2{ 400, 400 },*this);
	if (!ReadFile())
	{
		std::cout << "ファイルを読み込めませんでした。入力へ移行します。" << std::endl;
		state_ = UPDATE_STATE::HOST_IP;
		return true;
	}
	state_ = UPDATE_STATE::SELECT_HOST;
	return true;
}

// 共用 ネット使うかどうか、ホストかゲストか選択
bool LoginScene::SetNetWork(void)
{
	int x, y;
	GetMousePoint(&x, &y);
	Vector2 tst(x, y);
	bool flag = true;
	for (const auto& btn : btn_)
	{
		flag &= btn->Update(tst, (GetMouseInput() & MOUSE_INPUT_LEFT));
	}
	return flag;
}

// 共用 初期化してメッセージ飛ばす ホスト、ゲスト分けてもよかったけど大した量でもないし、ステート増えるしで分けなくていいかなって
bool LoginScene::StartInit(void)
{

	if (lpNetWork.GetMode() == NetWorkMode::HOST)
	{
		std::vector<int> ipInt;
		Vector2 pos(150, 150);
		DrawQue_.emplace_back(DrawQue{ pos ,{500,45}, 1.0,0,lpImageMng.GetID("IPBG")[0],0 });
		auto ipVec = lpNetWork.GetIP();
		for (auto& ip : ipVec)
		{
			if (ip.d1 != 0)
			{
				ChengeIPDATAToIntVector(ip.d1, ipInt);
				IPDraw(ipInt, pos, true);
				ChengeIPDATAToIntVector(ip.d2, ipInt);
				IPDraw(ipInt, pos, true);
				ChengeIPDATAToIntVector(ip.d3, ipInt);
				IPDraw(ipInt, pos, true);
				ChengeIPDATAToIntVector(ip.d4, ipInt);
				IPDraw(ipInt, pos, false);
			}
		}
		if (lpNetWork.GetActive() == ACTIVE_STATE::INIT)
		{
			if (!sendTmx_)
			{
				sendTmx_ = true;
				lpNetWork.SendTmxData("mapData/map.tmx");
				MesDataList list;
				lpNetWork.SendMesAll(MES_TYPE::STANBY_HOST, list);
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
				TRACE("スタンバイ完了\n");
				return true;
			}
		}
		if (lpNetWork.GetActive() == ACTIVE_STATE::STANBY && lpNetWork.GetStanbyPlayerNum() != 0)
		{
			std::cout << "開始" << std::endl;
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
			std::cout << "開始" << std::endl;
			return false;
		}
	}
	return true;
}

// ゲスト専用 前回入力したホストに接続するか新たに入力するかの選択
bool LoginScene::SelectHost(void)
{
	int select = 0;
	/*std::cout << "前回接続したホストに接続しますか？" << std::endl;
	std::cout << "1：前回接続したホストに接続する" << std::endl;
	std::cout << "2：新たにIPを入力する" << std::endl;*/
	/*std::cin >> select;*/
	int x, y;
	old = now;
	now = (GetMouseInput() & MOUSE_INPUT_LEFT);
	GetMousePoint(&x, &y);
	Vector2 tst(x, y);
	if (!numPad_->UpDate(tst, ((!old) && now)))
	{
		return true;
	}
	auto str = numPad_->GetInputStr();
	select = atoi(str.c_str());
	if (select == 1)
	{
		state_ = UPDATE_STATE::READ_HOST;
	}
	else if (select == 2)
	{
		state_ = UPDATE_STATE::HOST_IP;
		numPad_->init();
	}
	else
	{
		std::cout << "1または2を入力してください" << std::endl;
		numPad_->init();
	}
	return true;
}

// ゲスト専用 ファイルからのホストの読み込み ファイルがなかった場合、読み込んだIPアドレスで接続できなかった場合入力へ移行
bool LoginScene::ReadHost(void)
{
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
	std::fstream file("ini/IPData.dat", std::ios::binary | std::ios::in);

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
	std::fstream file("ini/IPData.dat", std::ios::binary | std::ios::out);

	if(!file)
	{
		return false;
	}

	file.write((char*)&ipData_, sizeof(ipData_));

	file.close();

	return true;
}

void LoginScene::ChengeIPDATAToIntVector(const unsigned char& ch,std::vector<int>& number)
{
	auto CheckDigit = [](const int& num)
	{
		int digit = 100;
		while (true)
		{
			if (num >= digit)
			{
				break;
			}
			digit /= 10;
			if (digit <= 0)
			{
				break;
			}
		}
		return digit;
	};
	number.clear();
	int num = ch;
	auto dig = CheckDigit(num);
	while (true)
	{
		number.emplace_back(num / dig);
		num %= dig;
		dig /= 10;
		if (dig <= 0)
		{
			break;
		}
	}
}

void LoginScene::IPDraw(const std::vector<int>& ipInt, Vector2& pos, bool comma)
{
	Vector2 size = { 33,49 };
	for (const auto& num : ipInt)
	{
		DrawQue_.emplace_back(DrawQue{ pos ,size,1.0,0.0,lpImageMng.GetID("Num")[num],1 });
		pos.x += size.x;
	}
	if (comma)
	{
		DrawQue_.emplace_back(DrawQue{ pos ,size,1.0,0.0,lpImageMng.GetID("Num")[10],1 });
		pos.x += size.x;
	}
}
