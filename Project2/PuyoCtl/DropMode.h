#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	bool operator ()(PleyErea& stage)
	{
		// —‚Æ‚·[
		stage.func_[STAGE_MODE::FALL](stage);
		// ‘€ì
		stage.playUnit_->Update();
		return true;
	}
};