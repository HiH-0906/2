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
			auto SetBit = [&](PUYO_ID id, Vector2 vec)
			{
				if (!stage.playErea_[vec.x][vec.y])
				{
					return false;
				}
				return stage.playErea_[vec.x][vec.y]->id() == id;
			};
			for (auto&& puyo : stage.puyoList_)
			{
				DirPermit bit;
				auto vec = puyo->GetGrid(stage.blockSize_);
				auto id = puyo->id();
				bit.perbit.up = SetBit(id, { vec.x,vec.y - 1 });
				bit.perbit.down = SetBit(id, { vec.x,vec.y + 1 });
				bit.perbit.left = SetBit(id, { vec.x - 1,vec.y });
				bit.perbit.right = SetBit(id, { vec.x + 1,vec.y });
				puyo->SetMunyonBit(bit);
				puyo->SetMuyonCnt();
			}
			stage.mode_ = STAGE_MODE::MUNYON;
		}
	}
};