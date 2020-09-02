#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"
#include "../ImageMng.h"

BaseScene::BaseScene() :fadeSpeed_(8)
{
	lpImageMng.GetID("‰¦", "image/‚Ó‚·‚Ü.png", Vector2{ 512,768 }, Vector2{ 2,1 });
	fadeScreen_ = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
	fadeScrID_ = MakeScreen(lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, true);
	fadePos_[0] = { -512,0 };
	fadePos_[1] = { 1024,0 };
	
	fadeCount_ = 0;
	startFCnt_ = lpSceneMng.fCnt();
}

BaseScene::~BaseScene()
{
}

void BaseScene::FadeInit(bool close)
{
	SetDrawScreen(DX_SCREEN_BACK);
	GetDrawScreenGraph(0, 0, lpSceneMng.screenSize().x, lpSceneMng.screenSize().y, fadeScrID_);
	fadeCount_ = 510;
	if (close)
	{
		fadePos_[0] = { 0,0 };
		fadePos_[1] = { 512,0 };
		fadeCount_ = 255;
	}
}

int BaseScene::FadeUpdate(void)
{
	int idbuff = GetDrawScreen();
	if (fadeCount_)
	{
		auto pos = lpSceneMng.screenSize() / 2;
		if (fadeCount_ > 255)
		{
			lpSceneMng.AddDrawList({ pos, fadeScrID_,1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
			if (fadePos_[0].x < 0)
			{
				fadePos_[0].x += fadeSpeed_;
				fadePos_[1].x -= fadeSpeed_;
			}
		}
		else
		{
			if (fadeCount_ < 1024 / fadeSpeed_)
			{
				fadePos_[0].x -= fadeSpeed_;
				fadePos_[1].x += fadeSpeed_;
			}
		}
		SetDrawScreen(fadeScreen_);
		ClsDrawScreen();
		DrawGraph(fadePos_[0].x, fadePos_[0].y, IMAGE_ID("‰¦")[1], true);
		DrawGraph(fadePos_[1].x, fadePos_[1].y, IMAGE_ID("‰¦")[0], true);
		lpSceneMng.AddDrawList({ pos ,fadeScreen_,1.0,0.0,1,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		fadeCount_ -= 2;
	}
	SetDrawScreen(idbuff);
	if (fadeCount_ < 0)
	{
		fadeCount_ = 0;
	}
	return fadeCount_;
}
