#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct OzyamaMode
{
	bool operator ()(PleyErea& stage)
	{
		stage.FallOzyama();
		// óéÇ∆Ç∑Å[
		stage.stageFunc_[STAGE_MODE::FALL](stage);
		stage.mode_ = STAGE_MODE::FALL;
		return true;
	}
};