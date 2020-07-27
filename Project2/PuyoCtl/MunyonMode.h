#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	void operator ()(PleyErea& stage)
	{
		if (cnt-- == 0)
		{
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
			cnt = 6;
		}
	}
	static int cnt;
};
int MunyonMode::cnt = 6;