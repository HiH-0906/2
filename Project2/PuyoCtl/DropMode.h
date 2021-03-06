#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	int operator ()(PleyErea& stage)
	{
		// 落とすー
		stage.stageFunc_[STAGE_MODE::FALL](stage);
		// 操作
		stage.playUnit_->Update();
		return 0;
	}
};