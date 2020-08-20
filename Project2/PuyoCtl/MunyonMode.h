#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	int operator ()(PleyErea& stage)
	{
		// ちょっとだけｲﾝﾀｰﾊﾞﾙ発生させてる
		bool muyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// むよーん
			puyo->Update();
			muyon |= puyo->CheckMuyonCnt();
		}
		if (!muyon)
		{
			// むよーんｶｳﾝﾄ後消去
			stage.mode_ = STAGE_MODE::ERASE;
		}
		return 0;
	}
};