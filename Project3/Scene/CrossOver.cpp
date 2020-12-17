#include <iostream>
#include <DxLib.h>
#include "CrossOver.h"
#include "SceneMng.h"
#include "../_debug/_DebugConOut.h"

CrossOver::CrossOver(uniqueBase old, uniqueBase next)
{
	cnt_ = 0;
	oldScene_ = std::move(old);
	nextScene_ = std::move(next);
	auto size = lpSceneMng.GetScreenSize();
	drawScreen_ = MakeScreen(size.x, size.y, true);
	TRACE("クロスオーバー" );
}

CrossOver::~CrossOver()
{
}

uniqueBase CrossOver::Update(uniqueBase own, const Time& now)
{
	if (cnt_ >= 255)
	{
		return std::move(nextScene_);
	}
	TransitionUpdate();
	return own;
}

void CrossOver::Draw(void)
{
	DrawGraph(0, 0, drawScreen_, true);
}

void CrossOver::Init(void)
{
}

void CrossOver::DrawOwnScene(void)
{
	SetDrawScreen(drawScreen_);
	ClsDrawScreen();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_);
	nextScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - cnt_);
	oldScene_->Draw();
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void CrossOver::TransitionUpdate(void)
{
	DrawOwnScene();
	cnt_++;
}
