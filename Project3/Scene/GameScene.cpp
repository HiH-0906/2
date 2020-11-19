#include <iostream>
#include <algorithm>
#include <DxLib.h>
#include "GameScene.h"
#include "CrossOver.h"
#include "LoginScene.h"
#include "SceneMng.h"
#include "../Obj/Player.h"
#include "../Obj/Bomb.h"
#include "../NetWork/NetWork.h"

uniqueBase GameScene::Update(uniqueBase own)
{
	objList_.sort([](shared_Obj& objA, shared_Obj& objB)
	{
		return objA->CheckMesList() > objB->CheckMesList();
	}
	);
	for (auto& pl:objList_)
	{
		pl->Update_();
	}
	auto delItr = std::remove_if(objList_.begin(), objList_.end(), [](shared_Obj& obj) {return !obj->Alive(); });
	objList_.erase(delItr, objList_.end());
	DrawOwnScene();
	DrawFps();
	if (lpNetWork.GetActive() == ACTIVE_STATE::NON)
	{
		mapMng_->EndOfMap();
		Player::fallCnt_ = 0;
		return std::make_unique<CrossOver>(std::move(own), std::make_unique<LoginScene>());
	}
	return own;
}

void GameScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	for (int i = 0; i < static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, mapMng_->GetDarwMap(static_cast<MapLayer>(i)), true);
	}
	for (auto& pl : objList_)
	{
		pl->Draw();
	}
}

void GameScene::Init(void)
{
	mapMng_ = std::make_shared<Map>();
	const auto& size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	const auto& mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{

		mapMng_->LoadMap("mapData/map2.tmx");
		const auto& cLayer = mapMng_->GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		const auto& chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,50 }, 4, id,mapMng_,*this));
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
		auto chip = mapMng_->GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,50 }, 4, id, mapMng_,*this));
				id += UNIT_ID_BASE;
			}
			cnt++;
		}
	}
	DrawOwnScene();
}

GameScene::GameScene()
{
	std::cout << "ƒQ[ƒ€ƒV[ƒ“" << std::endl;
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

void GameScene::SetBomb(Vector2 pos, int& id, int& oid, bool send, std::chrono::system_clock::time_point start)
{
	uinonTimeData time = { start };
	if (send)
	{
		sendData data[6];
		data[0].uidata = oid;
		data[1].uidata = id;
		data[2].uidata = pos.x;
		data[3].uidata = pos.y;
		data[4].uidata = time.idata[0];
		data[5].uidata = time.idata[1];
		
		lpNetWork.SendMes(MES_TYPE::SET_BOMB, { data[0],data[1],data[2],data[3],data[4],data[5] });
	}
	objList_.emplace_back(std::make_unique<Bomb>(pos, Vector2{ 32,32 }, id, oid, mapMng_, *this, start));
}
