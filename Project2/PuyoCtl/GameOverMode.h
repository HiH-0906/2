#pragma once
#include "../_debug/_DebugConOut.h"
#include "../PleyErea.h"

struct GameOverMode
{
	bool operator ()(PleyErea& stage)
	{
		return true;
	}
};