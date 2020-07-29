#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	bool operator ()(PleyErea& stage)
	{
		// ÇøÇÂÇ¡Ç∆ÇæÇØ≤›¿∞ ﬁŸî≠ê∂Ç≥ÇπÇƒÇÈ
		bool muyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// ÇﬁÇÊÅ[ÇÒ
			puyo->Update();
			muyon |= puyo->CheckMuyonCnt();
		}
		if (!muyon)
		{
			// ÇﬁÇÊÅ[ÇÒ≤›¿∞ ﬁŸå„≤›Ω¿›Ω
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
		return true;
	}
};