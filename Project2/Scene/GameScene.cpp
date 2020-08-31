#include <time.h>
#include <DxLib.h>
#include "GameScene.h"
#include "GameOverScene.h"
#include "Menu/MenuScene.h"
#include "Menu/MissedConScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../State/CON_ID.h"
#include "../EffectMng.h"
#include "../ImageMng.h"

GameScene::GameScene(PlayEreaVec&& playErea)
{
	playErea_ = playErea;
	srand((unsigned int)time(NULL));
	playPadNum_ = 0;
	startFCnt_ = lpSceneMng.fCnt();
	cntDownNum_ = 0;
	overFlag = false;
	lpImageMng.GetID("カウント", "image/start_mes.png", Vector2{ 510,182 }, Vector2{ 1,2 });
	for (auto erea:playErea_)
	{
		(*erea->GetInput())->Reset();
	}
	for (auto&& erea : playErea_)
	{
		if (erea->inputID() == CON_ID::PAD)
		{
			playPadNum_++;
		}
	}
	FadeInit(false);
}

GameScene::~GameScene()
{
}

unipueBase GameScene::Update(unipueBase own)
{
	_dbgStartDraw();
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });

	int reNum = 0;
	int fCnt = lpSceneMng.fCnt() - startFCnt_;

	if (fCnt / 60 % 2 && playPadNum_ != 0)
	{
		ReSetupJoypad();
	}
	// 接続PAD数がPADPlayerより少ない場合
	if (GetJoypadNum() < playPadNum_)
	{
		int screenImage = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
		SetDrawScreen(DX_SCREEN_BACK);
		GetDrawScreenGraph(0, 0, lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, screenImage);
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
		return std::make_unique<MissedConScene>(std::move(own), true, false, screenImage, playPadNum_);
	}
	cntDownNum_ = FadeUpdate();
	if (!cntDownNum_)
	{
		for (auto&& erea : playErea_)
		{

			reNum = erea->UpDate();
			if (!reNum == 0)
			{
				int id = erea->playerID() ^ 1;
				playErea_[id]->ozyamaCnt(reNum);
			}
			if (reNum == -1 && overFlag == false)
			{
				overFlag = true;
			}
			else if (reNum == -1 && overFlag == true)
			{
				return std::make_unique<GameOverScene>();
			}
			else
			{
				// 何もしない
			}
			if (erea->PlesePose())
			{
				int screenImage = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
				SetDrawScreen(DX_SCREEN_BACK);
				GetDrawScreenGraph(0, 0, lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, screenImage);
				lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, screenImage,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
				return std::make_unique<MenuScene>(std::move(own), true, false, screenImage, erea->GetInput());
			}
		}
	}
	if (cntDownNum_ > 152 && cntDownNum_ < 380)
	{
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, IMAGE_ID("カウント")[0],1.0,0.0,5,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	}
	else if (cntDownNum_ < 152 && cntDownNum_>62)
	{
		lpSceneMng.AddDrawList({ lpSceneMng.screenSize() / 2, IMAGE_ID("カウント")[1],1.0,0.0,5,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	}
	playErea_[0]->Draw();
	playErea_[1]->Draw();
	if (overFlag)
	{
		for (auto&& erea : playErea_)
		{
			erea->SetWinner(true);
		}
	}
	return std::move(own);
}
