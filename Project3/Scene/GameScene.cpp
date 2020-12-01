#include <iostream>
#include <algorithm>
#include <DxLib.h>
#include "GameScene.h"
#include "CrossOver.h"
#include "CheckeredBlock.h"
#include "LoginScene.h"
#include "SceneMng.h"
#include "../Obj/Player.h"
#include "../Obj/Bomb.h"
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugDispOut.h"

uniqueBase GameScene::Update(uniqueBase own, const Time& now)
{
	if (lpNetWork.GetActive() == ACTIVE_STATE::NON || (objList_.size() == 0) || lpNetWork.GetActive() == ACTIVE_STATE::OFFLINE)
	{
		mapMng_->ResrtOfMap();
		Player::fallCnt_ = 0;
		return std::move(std::make_unique<CheckeredBlock>(std::move(own), std::make_unique<LoginScene>()));
	}
	objList_.sort([](shared_Obj objA, shared_Obj objB)
	{
		return objA->CheckMesList() > objB->CheckMesList();
	}
	);
	if (state_ == GameState::PLAY)
	{
		for (auto& pl : objList_)
		{
			pl->Update_(now);
		}
	}
	auto delItr = std::remove_if(objList_.begin(), objList_.end(), [](shared_Obj& obj) {return !obj->Alive(); });
	objList_.erase(delItr, objList_.end());
	mapMng_->Update(now);
	DrawOwnScene();
	DrawFps(now);


	return own;
}

void GameScene::DrawOwnScene(void)
{
	const auto& size = mapMng_->GetMapSize();
	const auto& data = mapMng_->GetFlameData();
	const auto& chipSize = mapMng_->GetChipSize();
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	for (int i = 0; i < static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, mapMng_->GetDarwMap(static_cast<MapLayer>(i)), true);
	}
	mapMng_->DrawFlame();
	for (auto& pl : objList_)
	{
		pl->Draw();
	}
	cntDownFunc_[state_]();
}

void GameScene::Init(void)
{
	state_ = GameState::STY;
	mapMng_ = std::make_shared<Map>();
	const auto& size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	const auto& mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{

		mapMng_->LoadMap("mapData/map.tmx");
		const auto& cLayer = mapMng_->GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		const auto& chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,32 }, Vector2{ 32,51 }, 4, id, mapMng_, *this));
				id += UNIT_ID_BASE;
			}
			cnt++;
		}
	}
	else
	{
		mapMng_->LoadMap("Capture/test.tmx");
		drawScreen_ = MakeScreen(size.x, size.y, true);
		const auto& cLayer = mapMng_->GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		int max = lpNetWork.GetMax();
		auto chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,32 }, Vector2{ 32,51 }, 4, id, mapMng_, *this));
				id += UNIT_ID_BASE;
			}
			cnt++;

			if ((id / UNIT_ID_BASE) == max)
			{
				break;
			}
		}
	}
	lpImageMng.GetID("fire", "Image/fire.png", Vector2{ 32,32 }, Vector2{ 3,4 });
	initFunc();
	DrawOwnScene();
}

GameScene::GameScene()
{
	std::cout << "ゲームシーン" << std::endl;
	Init();
}

GameScene::~GameScene()
{
	objList_.clear();
}

shared_Obj GameScene::GetPlayer(int id)
{
	shared_Obj tmp = nullptr;
	for (auto& obj : objList_)
	{
		if (obj->GetID() == id)
		{
			tmp = obj;
			break;
		}
	}
	return tmp;
}

std::list<shared_Obj> GameScene::GetObjList(void)
{
	return objList_;
}

void GameScene::SetBomb(Vector2 pos, int& id, int& oid,  int length ,bool send, std::chrono::system_clock::time_point start)
{
	unionTimeData time = { start };
	auto chip = mapMng_->ChengeChip(pos);
	auto chipSize = mapMng_->GetChipSize();
	chip = chip * chipSize;
	if (send)
	{
		sendData data[7];
		data[0].uidata = oid;
		data[1].uidata = id;
		data[2].uidata = chip.x;
		data[3].uidata = chip.y;
		data[4].uidata = 3;
		data[5].uidata = time.idata[0];
		data[6].uidata = time.idata[1];
		
		lpNetWork.SendMes(MES_TYPE::SET_BOMB, { data[0],data[1],data[2],data[3],data[4],data[5],data[6] });
	}
	objList_.emplace_back(std::make_unique<Bomb>(chip, Vector2{ 32,32 }, length, id, oid, mapMng_, *this, start));
}

void GameScene::initFunc(void)
{
	cntDownFunc_.try_emplace(GameState::STY, [&]()
	{
		if (lpNetWork.GetGameStart())
		{
			state_ = GameState::COUNT;
		}
	});
	cntDownFunc_.try_emplace(GameState::COUNT, [&]()
	{
		auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - lpNetWork.GetCountDownTime()).count();
		cnt = abs(START_CNT - cnt);
		DrawFormatString(300, 500, 0xffffff, "開始まであと：%d秒", cnt / 1000);
		if (cnt / 1000 <= 0)
		{
			state_ = GameState::PLAY;
		}
	});
	cntDownFunc_.try_emplace(GameState::PLAY, [&]()
	{

	});
}
