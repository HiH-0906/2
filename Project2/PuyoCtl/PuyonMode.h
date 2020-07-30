#pragma once
#include "../PleyErea.h"

struct PuyonMode
{
	bool operator ()(PleyErea& stage)
	{
		// ぷよーんしてるやつがいるかどうか
		bool puyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// 一人でもいたら移行しない
			puyo->Update();
			puyon |= puyo->CheckPuyonCnt();
		}
		if (!puyon)
		{
			// むにょーんBitSet関数
			auto SetBit = [&](PUYO_ID id, Vector2 vec)
			{
				if (id == PUYO_ID::OZAYMA)
				{
					return false;
				}
				if (!stage.playErea_[vec.x][vec.y])
				{
					return false;
				}
				// つながっていいか？
				return stage.playErea_[vec.x][vec.y]->id() == id;
			};
			// ｹﾞｰﾑｵｰﾊﾞｰ位置判別
			if (stage.playErea_[4][1])
			{
				return false;
			}
			// むよーんﾁｪｯｸ
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
			// むよーん
			stage.mode_ = STAGE_MODE::MUNYON;
		}
		return true;
	}
};