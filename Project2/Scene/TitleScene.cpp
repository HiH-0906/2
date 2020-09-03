#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "../ImageMng.h"
#include "SceneMng.h"
#include "../SoundMng.h"
#include "GameScene.h"

bool TitleScene::close_ = true;

TitleScene::TitleScene()
{
	lpImageMng.GetID("Title", "image/Title.png");
	lpImageMng.GetID("Space", "image/space.png");
	cnt_ = 0;
	FadeInit(close_);
	if (close_)
	{
		close_ = false;
	}
	lpSoundMng.SetHandle("Title");
	lpSoundMng.SetHandle("‘¾ŒÛ");
	PlaySoundMem(lpSoundMng.GetHandle("Title"), DX_PLAYTYPE_LOOP);
}

TitleScene::~TitleScene()
{
}

unipueBase TitleScene::Update(unipueBase own)
{
	auto size = lpSceneMng.screenSize() / 2;
	lpSceneMng.AddDrawList(DrawQueT{ size,IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	auto cntDownNum_=FadeUpdate();
	if (cntDownNum_ > 255)
	{
		cntDownNum_ -= 510;
		ChangeVolumeSoundMem(255 - (abs(cntDownNum_)), lpSoundMng.GetHandle("Game"));
	}
	else if (cntDownNum_ != 0)
	{
		cntDownNum_ -= 255;
		ChangeVolumeSoundMem((abs(cntDownNum_)), lpSoundMng.GetHandle("Title"));
	}
	else
	{
		// ‰½‚à‚µ‚È‚¢
	}
	if (!cntDownNum_)
	{
		lpSceneMng.AddDrawList(DrawQueT{ {512,250},IMAGE_ID("Title")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			PlaySoundMem(lpSoundMng.GetHandle("‘¾ŒÛ"), DX_PLAYTYPE_BACK);
			return std::make_unique<SelectScene>();
		}
		if (cnt_ / 45 % 2)
		{
			lpSceneMng.AddDrawList(DrawQueT{ {512,500},IMAGE_ID("Space")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
		}
		cnt_++;
	}
	return std::move(own);
}