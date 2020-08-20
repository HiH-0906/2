#include <time.h>
#include "GameScene.h"
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "State/CON_ID.h"
#include "EffectMng.h"

GameScene::GameScene()
{
	srand((unsigned int)time(NULL));
	playErea_.emplace_back(std::make_unique<PleyErea>(Vector2{ 512, 768 }, Vector2{ 100,64 }, Vector2{ 0,0 }, CON_ID::KEY));
	playErea_.emplace_back(std::make_unique<PleyErea>(Vector2{ 512, 768 }, Vector2{ 100,64 }, Vector2{ 512, 0}, CON_ID::KEY));
}

GameScene::~GameScene()
{
}

void GameScene::Draw(void)
{
	for (size_t i = 0; i < playErea_.size(); i++)
	{
		DrawGraph(playErea_[i]->pos().x, playErea_[i]->pos().y, playErea_[i]->GetScreenID(), true);
	}
	lpEffectMng.Draw();
}

unipueBase GameScene::Update(unipueBase own)
{
	_dbgStartDraw();
	int reNum = 0;
	for (auto&& erea : playErea_)
	{
		reNum = erea->UpDate();
		if (!reNum == 0)
		{
			int id = erea->playerID() ^ 1;
			playErea_[id]->ozyamaCnt(reNum);
		}
	}
	if (reNum == -1)
	{
		for (auto&& erea : playErea_)
		{
			erea->SetWinner(true);
		}
	}
	return std::move(own);
}
