#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	int operator ()(PleyErea& stage)
	{
		// —‚Æ‚·[
		stage.stageFunc_[STAGE_MODE::FALL](stage);
		// ‘€ì
		stage.playUnit_->Update();
		return 0;
	}
};