#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

unipueBase TitleScene::Update(unipueBase own)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		return std::make_unique<SelectScene>();			// º∞›êÿÇËë÷Ç¶
	}
	return std::move(own);
}