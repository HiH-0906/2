#include <time.h>
#include <DxLib.h>
#include "GameScene.h"
#include "TitleScene.h"
#include "Menu/MenuScene.h"
#include "Menu/MissedConScene.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../State/CON_ID.h"
#include "../EffectMng.h"
#include "../ImageMng.h"
#include "../SoundMng.h"

GameScene::GameScene(PlayEreaVec&& playErea)
{
	playErea_ = playErea;
	srand((unsigned int)time(NULL));
	playPadNum_ = 0;
	cntDownNum_ = 0;
	overFlag = false;
	lpImageMng.GetID("カウント", "image/start_mes.png", Vector2{ 510,142 }, Vector2{ 1,2 });
	lpSoundMng.SetHandle("ぷにょん");
	lpSoundMng.SetHandle("泡");
	lpSoundMng.SetHandle("Game");
	PlaySoundMem(lpSoundMng.GetHandle("Game"), DX_PLAYTYPE_LOOP);
	ChangeVolumeSoundMem(0, lpSoundMng.GetHandle("Game"));
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
	cntDownNum_ = FadeUpdate();
	if (cntDownNum_ > 152 && cntDownNum_ < 380)
	{
		lpSceneMng.AddDrawList({ {lpSceneMng.screenSize().x / 2,lpSceneMng.screenSize().y / 3}, IMAGE_ID("カウント")[0],1.0,0.0,5,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	}
	else if (cntDownNum_ <= 152 && cntDownNum_ > 62)
	{
		lpSceneMng.AddDrawList({ {lpSceneMng.screenSize().x / 2,lpSceneMng.screenSize().y / 3}, IMAGE_ID("カウント")[1],1.0,0.0,5,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	}
	if (cntDownNum_ > 255)
	{
		cntDownNum_ -= 510;
		ChangeVolumeSoundMem(255 - (abs(cntDownNum_)), lpSoundMng.GetHandle("Title"));
	}
	else if (cntDownNum_ != 0)
	{
		cntDownNum_ -= 255;
		ChangeVolumeSoundMem((abs(cntDownNum_)), lpSoundMng.GetHandle("Game"));
	}
	else
	{
		// 何もしない
	}
	lpSceneMng.AddDrawList({ {512,384},IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });

	int reNum = 0;
	int fCnt = lpSceneMng.fCnt() - startFCnt_;

	if ((fCnt % 60) == 0 && playPadNum_ != 0)
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
	if (!cntDownNum_)
	{
		for (auto&& erea : playErea_)
		{

			reNum = erea->UpDate();
			if (reNum >= 0)
			{
				int id = erea->playerID() ^ 1;
				playErea_[id]->ozyamaCnt(reNum);
			}
			if (reNum == -1)
			{
				overFlag = true;
			}
			if (reNum == -2)
			{
				lpSceneMng.DrawPanel(SCREEN_ID::FRONT, 200, 0x000000, 0);
				if ((lpSceneMng.fCnt() - startFCnt_) / 45 % 2)
				{
					lpSceneMng.AddDrawList(DrawQueT{ {512,600},IMAGE_ID("Space")[0],1.0,0.0,5,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
				}
				if (CheckHitKey(KEY_INPUT_SPACE))
				{
					PlaySoundMem(lpSoundMng.GetHandle("太鼓"), DX_PLAYTYPE_BACK);
					return std::make_unique<TitleScene>();
				}
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
