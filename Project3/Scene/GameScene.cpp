#include <iostream>
#include <DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"

uniqueBase GameScene::Update(uniqueBase own)
{
	for (auto& pl:objList_)
	{
		pl->Update_();
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
	for (auto& pl : objList_)
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
		auto cLayer = map.GetMapData(MapLayer::CHAR);
		int cnt = 0;
		int id = 0;
		auto chip = map.GetChipSize();
		for (auto data : cLayer)
		{
			if (data != -1)
			{
				Vector2 pos = { chip.x * (cnt % 21),chip.y * (cnt / 21) };
				objList_.emplace_back(std::make_shared<Player>(pos, Vector2{ 32,50 }, 4, id));
				MesDataList mes;
				sendData data;
				data.idata = id;
				mes.emplace_back(data);
				data.idata = pos.x;
				mes.emplace_back(data);
				data.idata = pos.y;
				mes.emplace_back(data);
				lpNetWork.SendMes(MES_TYPE::INSTANCE, mes);
				id++;
			}
			cnt++;
		}
	}
	else
	{
		map.LoadMap("Capture/test.tmx");
		RevData mes;
		while (!lpNetWork.CheckMes(MES_TYPE::INSTANCE))
		{
			
		}
		while (mes.first.type == MES_TYPE::INSTANCE)
		{
			MesDataList data = mes.second;
			objList_.emplace_back(std::make_shared<Player>(Vector2{ static_cast<int>(data[1].idata),static_cast<int>(data[2].idata) }, Vector2{ 32,50 }, 4, data[0].idata));
			mes = lpNetWork.PickUpMes();
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
}
