#pragma once
#include "../PleyErea.h"
#include "FallMode.h"

struct DropMode 
{
	bool operator ()(PleyErea& stage)
	{
		// ���Ƃ��[
		stage.func_[STAGE_MODE::FALL](stage);
		// ����
		stage.playUnit_->Update();
		return true;
	}
};