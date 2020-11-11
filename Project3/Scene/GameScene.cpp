#include <iostream>
#include <DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"

uniqueBase GameScene::Update(uniqueBase own)
{
	for (auto pl:plList_)
	{
		pl->Update();
	}
	DrawOwnScene();
	return own;
}

void GameScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	for (int i = 0; i <= static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, Map::GetInstance().GetDarwMap(static_cast<MapLayer>(i)), true);
	}
	for (auto pl : plList_)
	{
		pl->Draw();
	}
}

void GameScene::Init(void)
{
	auto& map = Map::GetInstance();
	auto size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	auto mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{
		map.LoadMap("mapData/map2.tmx");
	}
	else
	{
		map.LoadMap("Capture/test.tmx");
	}
	plList_.emplace_back(std::make_shared<Player>(map.GetChipSize(), 1));
	DrawOwnScene();
}

GameScene::GameScene()
{
	std::cout << "ƒQ[ƒ€ƒV[ƒ“" << std::endl;
	Init();
}

GameScene::~GameScene()
{
}
