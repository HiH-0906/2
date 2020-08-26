#pragma once
#include "../_debug/_DebugConOut.h"
#include "../PleyErea.h"

struct GameOverMode
{
	int operator ()(PleyErea& stage)
	{
		auto pos = stage.pos_ + (stage.size_ / 2);
		lpSceneMng.AddDrawList({ pos,IMAGE_ID("LOSE")[0],1.0,0.0,0,SCREEN_ID::FRONT ,DATA_TYPE::IMG ,true });
		return 0;
	}
};