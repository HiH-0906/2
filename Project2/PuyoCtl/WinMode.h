#pragma once
#include "../_debug/_DebugConOut.h"
#include "../PleyErea.h"

struct WinMode
{
	int operator ()(PleyErea& stage)
	{
		auto pos = stage.pos_ + (stage.size_ / 2);
		lpSceneMng.DrawPanel(SCREEN_ID::FRONT, 200,0x000000,0);
		lpSceneMng.AddDrawList({ pos,IMAGE_ID("WIN")[0],1.0,0.0,5,SCREEN_ID::FRONT ,DATA_TYPE::IMG ,true });
		if ((*stage.GetInput()[stage.inputID()])->GetKeyTrg(INPUT_ID::RROTA))
		{
			return -2;
		}
		return 0;
	}
};