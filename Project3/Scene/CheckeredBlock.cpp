#include <DxLib.h>
#include "CheckeredBlock.h"
#include "SceneMng.h"

CheckeredBlock::CheckeredBlock(uniqueBase old, uniqueBase next) 
{
	divCnt_ = Vector2{ 32,32 };
	nextBlockList_ = {};
	auto screensize = lpSceneMng.GetScreenSize();
	oldScene_ = std::move(old);
	nextScene_ = std::move(next);
	cnt_ = 0;
	oldImage_ = MakeScreen(screensize.x, screensize.y, true);
	SetDrawScreen(oldImage_);
	oldScene_->Draw();
	MakeBlockImage(oldImage_,false,oldBlockList_);
	nextImage_ = MakeScreen(screensize.x, screensize.y, true);
	SetDrawScreen(nextImage_);
	nextScene_->Draw();
	drawScreen_ = MakeScreen(screensize.x, screensize.y, true);
	MakeBlockImage(nextImage_,true,nextBlockList_);
	DrawOwnScene();
}

CheckeredBlock::~CheckeredBlock()
{
}

uniqueBase CheckeredBlock::Update(uniqueBase own, const Time& now)
{
	if (cnt_ >= 255)
	{
		return std::move(nextScene_);
	}
	TransitionUpdate();
	return own;
}

void CheckeredBlock::TransitionUpdate(void)
{
	DrawOwnScene();
	cnt_++;
}

void CheckeredBlock::Draw(void)
{
	DrawGraph(0, 0, drawScreen_, true);
}

void CheckeredBlock::Init(void)
{
}

void CheckeredBlock::DrawOwnScene(void)
{
	auto size = Vector2{ static_cast<int>(ceil(lpSceneMng.GetScreenSize().x / divCnt_.x)), static_cast<int>(ceil(lpSceneMng.GetScreenSize().y / divCnt_.y)) };
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	for (int y = 0; y < divCnt_.y; y++)
	{
		for (int x = 0; x < divCnt_.x; x++)
		{
			if ((x + y) < (cnt_ / 4))
			{
				DrawGraph(size.x * x, size.y * y, nextBlockList_[x + y * divCnt_.x], true);
			}
			else
			{
				DrawGraph(size.x * x, size.y * y, oldBlockList_[x + y * divCnt_.x], false);
			}
		}
	}
}

void CheckeredBlock::MakeBlockImage(const int& image, bool next,std::vector<int>& list)
{
	auto size = lpSceneMng.GetScreenSize() / divCnt_;
	Vector2 pos = {};
	for (int y = 0; y < divCnt_.y; y++)
	{
		for (int x = 0; x < divCnt_.x; x++)
		{
			pos = { size.x * x,size.y * y };
			int img = DerivationGraph(pos.x, pos.y, size.x, size.y, image);
			list.emplace_back(img);
		}
	}
}
