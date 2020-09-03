#pragma once
#include "../_debug/_DebugConOut.h"
#include "../PleyErea.h"

struct GameOverMode
{
	int speed = 8;
	Vector2 pos = {};
	int operator ()(PleyErea& stage)
	{
		if (pos.x == 0 && pos.y == 0)
		{
			pos = stage.pos_ + (stage.size_ / 2);
		}
		stage.rad_ += atanf(0.05f);
		stage.pos_.y += speed;
		lpSceneMng.AddDrawList({ pos,IMAGE_ID("LOSE")[0],1.0,0.0,5,SCREEN_ID::FRONT ,DATA_TYPE::IMG ,true });
		return -2;
	}
};