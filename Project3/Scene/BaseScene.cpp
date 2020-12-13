#include <DxLib.h>
#include "BaseScene.h"
#include "SceneMng.h"
#include "../Obj/Player.h"
#include "../_debug/_DebugConOut.h"

BaseScene::BaseScene()
{
	drawScreen_ = 0;
	strat_ = std::chrono::system_clock::now();
	end_ = strat_;
	fpsCnt_ = 0;
	fps_ = 0;
	secondCnt_ = 0;
}

BaseScene::~BaseScene()
{
}

void BaseScene::Draw(void)
{
	DrawGraph(0, 0, drawScreen_, true);
}

void BaseScene::DrawFps(const Time& now)
{
	end_ = now;
	if (std::chrono::duration_cast<std::chrono::seconds>(end_ - strat_).count() < 1)
	{
		fpsCnt_++;
	}
	else
	{
		strat_ = end_;
		fps_ = fpsCnt_;
		secondCnt_++;
		fpsCnt_ = 0;
	}

	DrawBox(0, 0, 80, 16, 0x000000, true);
	DrawFormatString(0, 0, 0xffffff, "FPS：1/%d", fps_);

	DrawBox(128, 0, 300, 16, 0x000000, true);
	DrawFormatString(128, 0, 0xffffff, "データ欠落平均値：%d", (Player::fallCnt_ / secondCnt_));
}

void BaseScene::AddDrawQue(const DrawQue& que)
{
	DrawQue_.emplace_back(que);
}
