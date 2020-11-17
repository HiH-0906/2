#include <iostream>
#include <DxLib.h>
#include "GameScene.h"
#include "CrossOver.h"
#include "LoginScene.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"

uniqueBase GameScene::Update(uniqueBase own)
{
	for (auto& pl:objList_)
	{
		pl->Update_();
	}
	DrawOwnScene();
	DrawFps();
	if (lpNetWork.GetActive() == ACTIVE_STATE::NON)
	{
		Map::GetInstance().EndOfMap();
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
		DrawGraph(0, 0, Map::GetInstance().GetDarwMap(static_cast<MapLayer>(i)), true);
	}
	for (auto& pl : objList_)
	{
		pl->Draw();
	}
}

void GameScene::Init(void)
{
	auto& map = Map::GetInstance();
	const auto& size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	const auto& mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{

		map.LoadMap("mapData/map2.tmx");
		const auto& cLayer = map.GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		const auto& chip = map.GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,50 }, 4, id));
				id++;
			}
			cnt++;
		}
	}
	else
	{
		map.LoadMap("Capture/test.tmx");
		drawScreen_ = MakeScreen(size.x, size.y, true);
		const auto& cLayer = map.GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		auto chip = map.GetChipSize();
		for (const auto& data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,50 }, 4, id));
				id++;
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
