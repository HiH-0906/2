#pragma once
#include <DxLib.h>
#include "../PleyErea.h"
#include "../SoundMng.h"

struct FallMode
{
	int operator ()(PleyErea& stage)
	{
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
			// 動いていいー？
			stage.CheckMovePuyo(puyo);
		}
		);
		bool eraseFlag = true;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
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
			PlaySoundMem(lpSoundMng.GetHandle("ぷにょん"), DX_PLAYTYPE_BACK);
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
						// ぷよーんしないぷよが降ってきていた場合しない
						if (!stage.playErea_[vec.x][y]->SetPuyonCnt())
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
				// ぷよーんは自分含めて３人までなんだ…
				if (++cnt >= 3)
				{
					break;
				}
			}
		}
		return 0;
	}
};