#include<DxLib.h>
#include "NextPuyoCtl.h"
#include "Scene/SceneMng.h"
#include "ImageMng.h"

NextPuyoCtl::NextPuyoCtl(Vector2& pos, int haveCount, int drawCount)
{
	screenID_ = MakeScreen(48, 80, true);
	pos_ = pos;
	drawCount_ = drawCount;
	lpImageMng.GetID("nextBG", "image/nextbg.png");
	lpImageMng.GetID("nextFream", "image/nextFream.png");
	Add(haveCount);
}

NextPuyoCtl::~NextPuyoCtl()
{
}

void NextPuyoCtl::Draw(void)
{
	auto idBefor = GetDrawScreen();
	SetDrawScreen(screenID_);
	for (auto pair : nextPuyoList_)
	{
		DrawOval(pair.first->pos().x, pair.first->pos().y, 16, 16, pair.first->GetColor(), true);
		DrawOval(pair.second->pos().x, pair.second->pos().y, 16, 16, pair.second->GetColor(), true);
	}

	lpSceneMng.AddDrawList({ pos_,screenID_,1.0,0.0,0,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ pos_,IMAGE_ID("nextBG")[0],1.0,0.0,-1,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	lpSceneMng.AddDrawList({ pos_,IMAGE_ID("nextFream")[0],1.0,0.0,-1,SCREEN_ID::PLAY,DATA_TYPE::IMG,true });
	SetDrawScreen(idBefor);
}

NextPair NextPuyoCtl::PickUp(void)
{
	auto data = *(nextPuyoList_.begin());
	nextPuyoList_.erase(nextPuyoList_.begin());
	Add(1);
	return data;
}

bool NextPuyoCtl::Add(int no)
{
	while (no)
	{
		nextPuyoList_.emplace_back(
			NextPair{ 
				std::make_shared<Puyo>(Vector2{ 0,0 }, static_cast<PUYO_ID>((rand() % 5) + 1)),
				std::make_shared<Puyo>(Vector2{ 0,0 }, static_cast<PUYO_ID>((rand() % 5) + 1))
			}
		);
		no--;
	}
	Vector2 offSet = { 32,16 };
	Vector2 pos1 = { 16,16 };
	Vector2 pos2 = { 16,48 };
	for (auto pair:nextPuyoList_)
	{
		pair.first->pos(std::move(pos1));
		pair.second->pos(std::move(pos2));
		pos1 += offSet;
		pos2 += offSet;
	}
	return true;
}
