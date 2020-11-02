#include <DxLib.h>
#include "GameScene.h"
#include "SceneMng.h"
#include "../NetWork/NetWork.h"

unipueBase GameScene::Update(unipueBase own)
{
	DrawOwnScene();
	return own;
}

void GameScene::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	for (int i = 0; i <= static_cast<int>(MapLayer::CHAR); i++)
	{
		DrawGraph(0, 0, mapMng_->GetDarwMap(static_cast<MapLayer>(i)), true);
	}
}

void GameScene::Init(void)
{
	auto size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	auto mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::HOST || mode == NetWorkMode::OFFLINE)
	{
		mapMng_ = std::make_unique<Map>("Capture/test.tmx");
	}
	else
	{
		mapMng_ = std::make_unique<Map>("Capture/test.tmx");
	}
	DrawOwnScene();
}

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}
