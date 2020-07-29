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
			// ぷよーんし終わったら消えてもらいます
			stage.mode_ = STAGE_MODE::ERASE;
		}
		return true;
	}
};