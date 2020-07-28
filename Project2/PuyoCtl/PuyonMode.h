#pragma once
#include "../PleyErea.h"

struct PuyonMode
{
	void operator ()(PleyErea& stage)
	{
		bool puyon = false;
		for (auto puyo : stage.puyoList_)
		{
			puyo->Update();
			puyon |= puyo->CheckPuyonCnt();
		}
		if (!puyon)
		{
			stage.mode_ = STAGE_MODE::ERASE;
		}
	}
};