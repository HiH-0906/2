#include <DxLib.h>
#include "GameOverScene.h"
#include "TitleScene.h"

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

unipueBase GameOverScene::Update(unipueBase own)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<TitleScene>();			// º∞›êÿÇËë÷Ç¶
	}
	return std::move(own);
}
