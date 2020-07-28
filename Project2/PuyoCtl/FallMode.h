#pragma once
#include "../PleyErea.h"

struct FallMode
{
	void operator ()(PleyErea& stage)
	{
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			auto vec = puyo->GetGrid(stage.blockSize_);
			stage.playErea_[vec.x][vec.y] = puyo;					// id“ü‚ê‚Ä`
			DirPermit per;
			per.per = 0;
			if (!stage.CheckMovePuyo(puyo))
			{
				stage.playErea_[vec.x][vec.y].reset();
				puyo->SetMunyonBit(per);
				puyo->SetMuyonCnt();
			}
		});
		bool eraseFlag = true;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			if (!puyo->Update())
			{
				eraseFlag = false;
			}
		});
		if (eraseFlag)
		{
			stage.mode_ = STAGE_MODE::ERASE;
		}
	}
};