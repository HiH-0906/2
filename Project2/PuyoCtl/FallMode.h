#pragma once
#include "../PleyErea.h"

struct FallMode
{
	bool operator ()(PleyErea& stage)
	{
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			// 動いていいー？
			if (!stage.CheckMovePuyo(puyo))
			{
				// いい場合書き込んである情報削除
				auto vec = puyo->GetGrid(stage.blockSize_);
				stage.playErea_[vec.x][vec.y].reset();
			}
		});
		bool eraseFlag = true;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			// 動いてるやつがいる場合Eraseに移行しない
			if (!puyo->Update())
			{
				eraseFlag = false;
			}
		});
		if (eraseFlag)
		{
			// 消す前にぷよーん
			stage.mode_ = STAGE_MODE::PUYON;
		}
		for (auto puyo : stage.puyoList_)
		{
			// 今落ちてきたぷよかどうか
			if (!puyo->GetDownNow())
			{
				continue;
			}
			auto vec = puyo->GetGrid(stage.blockSize_);
			int cnt = 0;
			for (int y = vec.y; y < stage.stgSize_.y; y++)
			{
				if (stage.playErea_[vec.x][y])
				{
					// 壁はぷよーんしません
					if (stage.playErea_[vec.x][y]->id() != PUYO_ID::WALL)
					{
						// ぷよーん
						stage.playErea_[vec.x][y]->SetPuyonCnt();
					}
				}
				// ぷよーんは自分含めて３人までなんだ…
				if (++cnt >= 3)
				{
					break;
				}
			}
		}
		return true;
	}
};