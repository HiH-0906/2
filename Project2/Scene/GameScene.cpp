#include <time.h>
#include "GameScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../State/CON_ID.h"
#include "../EffectMng.h"
#include "../ImageMng.h"

GameScene::GameScene()
{
	srand((unsigned int)time(NULL));
	playErea_.emplace_back(std::make_unique<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 0,0 }, CON_ID::KEY));
	playErea_.emplace_back(std::make_unique<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 512, 0}, CON_ID::KEY));
	lpImageMng.GetID("BG", "image/bg2.png");
}

GameScene::~GameScene()
{
}

unipueBase GameScene::Update(unipueBase own)
{
	_dbgStartDraw();
	lpSceneMng.AddDrawList({ {256,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ {768,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	int reNum = 0;
	bool overFlag = false;
	for (auto&& erea : playErea_)
	{
		reNum = erea->UpDate();
		if (!reNum == 0)
		{
			int id = erea->playerID() ^ 1;
			playErea_[id]->ozyamaCnt(reNum);
		}
		if (reNum == -1)
		{
			overFlag = true;
		}
	}
	if (overFlag)
	{
		for (auto&& erea : playErea_)
		{
			erea->SetWinner(true);
		}
	}
	return std::move(own);
}
