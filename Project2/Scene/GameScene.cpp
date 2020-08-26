#include <time.h>
#include <DxLib.h>
#include "GameScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../State/CON_ID.h"
#include "../EffectMng.h"
#include "../ImageMng.h"

GameScene::GameScene()
{
	srand((unsigned int)time(NULL));
	playErea_ = lpSceneMng.playErea();
	playPadNum_ = 0;
	poseFlag_ = false;
	startCnt_ = lpSceneMng.fCnt();
	for (auto&& erea : playErea_)
	{
		if (erea->inputID() == CON_ID::PAD)
		{
			playPadNum_++;
		}
	}
}

GameScene::~GameScene()
{
}

unipueBase GameScene::Update(unipueBase own)
{
	_dbgStartDraw();
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	int reNum = 0;
	bool overFlag = false;
	int fCnt = lpSceneMng.fCnt() - startCnt_;
	if (fCnt / 30 % 2)
	{
		ReSetupJoypad();
	}
	if (GetJoypadNum() < playPadNum_)
	{
		poseFlag_ = true;
	}
	if (poseFlag_)
	{
		lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
		for (int i = 1; i <= playPadNum_; i++)
		{
			if (GetJoypadInputState(i) & PAD_INPUT_8)
			{
				poseFlag_ = false;
			}
		}
		return std::move(own);
	}
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
