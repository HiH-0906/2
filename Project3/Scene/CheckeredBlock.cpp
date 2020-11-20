#include <DxLib.h>
#include "CheckeredBlock.h"
#include "SceneMng.h"

CheckeredBlock::CheckeredBlock(uniqueBase old, uniqueBase next, Vector2 size) :divCnt_(size)
{
	blockList_ = {};
	auto screensize = lpSceneMng.GetScreenSize();
	oldScene_ = std::move(old);
	nextScene_ = std::move(next);
	cnt_ = 0;
	image_ = MakeScreen(screensize.x, screensize.y, true);
	SetDrawScreen(image_);
	nextScene_->Draw();
	drawScreen_ = MakeScreen(screensize.x, screensize.y, true);
	MakeBlockImage();
	DrawOwnScene();
}

CheckeredBlock::~CheckeredBlock()
{
}

uniqueBase CheckeredBlock::Update(uniqueBase own)
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
	auto size = lpSceneMng.GetScreenSize() / divCnt_;
	SetDrawScreen(drawScreen_);
	oldScene_->Draw();
	if (cnt_ > 128)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_ * 2 - 256);
		for (int y = 0; y < divCnt_.y; y++)
		{
			for (int x = 0; x < divCnt_.x; x++)
			{
				if ((x + y) % 2 == 1)
				{
					DrawGraph(size.x * x, size.y * y, blockList_[x + y * divCnt_.x], true);
				}
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_ * 2);
	for (int y = 0; y < divCnt_.y; y++)
	{
		for (int x = 0; x < divCnt_.x; x++)
		{
			if ((x + y) % 2 == 0)
			{
				DrawGraph(size.x * x, size.y * y, blockList_[x + y * divCnt_.x], true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void CheckeredBlock::MakeBlockImage(void)
{
	auto size = lpSceneMng.GetScreenSize() / divCnt_;
	Vector2 pos = {};
	for (int y = 0; y < divCnt_.y; y++)
	{
		for (int x = 0; x < divCnt_.x; x++)
		{
			pos = { size.x * x,size.y * y };
			int img = DerivationGraph(pos.x, pos.y, size.x, size.y, image_);
			blockList_.emplace_back(img);
		}
	}
}
