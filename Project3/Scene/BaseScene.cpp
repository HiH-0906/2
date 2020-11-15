#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"

BaseScene::BaseScene()
{
	drawScreen_ = 0;
	strat_ = std::chrono::system_clock::now();
	end_ = std::chrono::system_clock::now();
	fpsCnt_ = 0;
	fps_ = 0;
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, drawScreen_, true);
}

void BaseScene::Fps(void)
{
	end_ = std::chrono::system_clock::now();
	if (std::chrono::duration_cast<std::chrono::seconds>(end_ - strat_).count() < 1)
	{
		fpsCnt_++;
	}
	else
	{
		strat_ = std::chrono::system_clock::now();
		fps_ = fpsCnt_;
		fpsCnt_ = 0;
	}
	DrawBox(0, 0, 64, 16, 0x000000, true);
	DrawFormatString(0, 0, 0xffffff, "1/%d", fps_);
}
