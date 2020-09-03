#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "../ImageMng.h"
#include "SceneMng.h"
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
}

TitleScene::~TitleScene()
{
}

unipueBase TitleScene::Update(unipueBase own)
{
	auto size = lpSceneMng.screenSize() / 2;
	lpSceneMng.AddDrawList(DrawQueT{ size,IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		lpSceneMng.AddDrawList(DrawQueT{ {512,250},IMAGE_ID("Title")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
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