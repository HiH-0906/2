#pragma once
#include "../PleyErea.h"

struct EraseMode
{
	void operator ()(PleyErea& stage)
	{
		bool delFlag = false;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			delFlag |= stage.SetErasePuyo(puyo->GetGrid(stage.blockSize_), puyo->id());
		});
		if (delFlag)
		{
			stage.DeletePuyo();
			stage.mode_ = STAGE_MODE::FALL;
		}
		else
		{
			auto SetBit = [&](PuyoUnit puyo, Vector2 vec)
			{
			};
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
	}
};