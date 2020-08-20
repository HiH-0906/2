#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	int operator ()(PleyErea& stage)
	{
		// ������Ƃ���������ٔ��������Ă�
		bool muyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// �ނ�[��
			puyo->Update();
			muyon |= puyo->CheckMuyonCnt();
		}
		if (!muyon)
		{
			// �ނ�[���Č����
			stage.mode_ = STAGE_MODE::ERASE;
		}
		return 0;
	}
};