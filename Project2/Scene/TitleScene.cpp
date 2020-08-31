#include <DxLib.h>
#include "TitleScene.h"
#include "SelectScene.h"
#include "../ImageMng.h"
#include "SceneMng.h"
#include "GameScene.h"

bool TitleScene::close_ = true;

TitleScene::TitleScene()
{
	playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 0,0 }, CON_ID::KEY));
	playErea_.emplace_back(std::make_shared<PleyErea>(Vector2{ 512, 768 }, Vector2{ 75,128 }, Vector2{ 512, 0 }, CON_ID::KEY));
	lpImageMng.GetID("Title", "image/Title.png");
	lpImageMng.GetID("start", "image/Title.png");
	button_.emplace_back(std::make_shared<Button>(Vector2{ 512,462 }, 0.0f, 1.0f, "start"));
	button_.emplace_back(std::make_shared<Button>(Vector2{ 512,562 }, 0.0f, 1.0f, "start"));
	cursorNum_ = 0;
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
	lpSceneMng.AddDrawList(DrawQueT{ {512,250},IMAGE_ID("Title")[0],1.0,0.0,0,SCREEN_ID::FRONT,DATA_TYPE::IMG,true });
	if (!FadeUpdate())
	{
	// �ׂ�������������������
		for (int i = 0; i < playErea_.size(); i++)
		{
			(*playErea_[i]->GetInput())->Update();

			if ((*playErea_[i]->GetInput())->GetKeyTrg(INPUT_ID::UP))
			{
				cursorNum_--;
				if (cursorNum_ < 0)
				{
					cursorNum_ = button_.size() - 1;
				}
			}
			if ((*playErea_[i]->GetInput())->GetKeyTrg(INPUT_ID::DOWN))
			{
				cursorNum_++;
				if (cursorNum_ >= button_.size())
				{
					cursorNum_ = 0;
				}
			}
			for (auto&& button : button_)
			{
				if (button->Update(button_[cursorNum_]->pos(), true) && (*playErea_[i]->GetInput())->GetKeyTrg(INPUT_ID::RROTA))
				{
					return std::make_unique<GameScene>(std::move(playErea_));			// ��ݐ؂�ւ�
				}
			}
		}
	}
	return std::move(own);
}