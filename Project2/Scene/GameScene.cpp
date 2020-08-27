#include <time.h>
#include <DxLib.h>
#include "GameScene.h"
#include "Menu/MenuScene.h"
#include "Menu/MissedConScene.h"
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
	if (CheckHitKey(KEY_INPUT_F2))
	{
		int screenImage = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y,true);
		SetDrawScreen(DX_SCREEN_BACK);
		GetDrawScreenGraph(0, 0, lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, screenImage);
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
		return std::make_unique<MenuScene>(std::move(own), true, false, screenImage);
	}
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	int reNum = 0;
	bool overFlag = false;
	int fCnt = lpSceneMng.fCnt() - startCnt_;
	if (fCnt / 60 % 2)
	{
		ReSetupJoypad();
	}
	if (GetJoypadNum() < playPadNum_)
	{
		int screenImage = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
		SetDrawScreen(DX_SCREEN_BACK);
		GetDrawScreenGraph(0, 0, lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, screenImage);
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
		return std::make_unique<MissedConScene>(std::move(own), true, false, screenImage,playPadNum_);
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