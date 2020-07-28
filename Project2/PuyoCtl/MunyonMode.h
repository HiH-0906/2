#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	void operator ()(PleyErea& stage)
	{
		bool muyon = false;
		for (auto puyo : stage.puyoList_)
		{
			puyo->Update();
			muyon |= puyo->CheckMuyonCnt();
		}
		if (!muyon)
		{
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
	}
};