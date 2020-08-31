#include <DxLib.h>
#include "GameOverScene.h"
#include "../ImageMng.h"
#include "SceneMng.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
	FadeInit(false);
}

GameOverScene::~GameOverScene()
{
}

unipueBase GameOverScene::Update(unipueBase own)
{
	auto size = lpSceneMng.screenSize() / 2;
	lpSceneMng.AddDrawList(DrawQueT{ size,IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			return std::make_unique<TitleScene>();			// º∞›êÿÇËë÷Ç¶
		}
	}
	return std::move(own);
}
