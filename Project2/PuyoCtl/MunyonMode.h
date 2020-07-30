#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	bool operator ()(PleyErea& stage)
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
			if (stage.ozyamaCnt_)
			{
				stage.mode_ = STAGE_MODE::OZYAMA;
				return true;
			}
			{
				// むよーんｲﾝﾀｰﾊﾞﾙ後ｲﾝｽﾀﾝｽ
				stage.InstancePuyo();
				stage.CheckMovePuyo(stage.puyoList_[0]);
				stage.mode_ = STAGE_MODE::DROP;
			}
		}
		return true;
	}
};