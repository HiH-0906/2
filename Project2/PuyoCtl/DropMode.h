#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	int operator ()(PleyErea& stage)
	{
		// ���Ƃ��[
		stage.stageFunc_[STAGE_MODE::FALL](stage);
		// ����
		stage.playUnit_->Update();
		return 0;
	}
};