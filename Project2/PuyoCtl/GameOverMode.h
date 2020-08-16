#pragma once
#include "../PleyErea.h"

struct GameOverMode
{
	bool operator ()(PleyErea& stage)
	{
		return true;
	}
};