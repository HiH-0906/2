#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "../ImageMng.h"
#include "SceneMng.h"

TitleScene::TitleScene()
{
	lpImageMng.GetID("Title", "image/Title.png");
	button_.emplace_back(std::make_shared<Button>(Vector2{ 512,512 }, Vector2{ 256,128 }));
}

TitleScene::~TitleScene()
{
}

unipueBase TitleScene::Update(unipueBase own)
{
	Vector2 pos;
	GetMousePoint(&pos.x, &pos.y);
	auto size = lpSceneMng.screenSize() / 2;
	lpSceneMng.AddDrawList(DrawQueT{ size,IMAGE_ID("BG")[0],1.0,0.0,0,SCREEN_ID::BG,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList(DrawQueT{ {512,250},IMAGE_ID("Title")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	for (auto&& button : button_)
	{
		if (button->Update(pos, true) && GetMouseInput() & MOUSE_INPUT_LEFT)
		{
			return std::make_unique<SelectScene>();			// º∞›êÿÇËë÷Ç¶
		}
		
	}
	
	return std::move(own);
}