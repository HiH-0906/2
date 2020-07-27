#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	void operator ()(PleyErea& stage)
	{
		stage.func_[STAGE_MODE::FALL](stage);
		stage.playUnit_->Update();
	}
};