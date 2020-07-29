#pragma once
#include "../PleyErea.h"

struct PuyonMode
{
	bool operator ()(PleyErea& stage)
	{
		// �Ղ�[�񂵂Ă������邩�ǂ���
		bool puyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// ��l�ł�������ڍs���Ȃ�
			puyo->Update();
			puyon |= puyo->CheckPuyonCnt();
		}
		if (!puyon)
		{
			// �Ղ�[�񂵏I�����������Ă��炢�܂�
			stage.mode_ = STAGE_MODE::ERASE;
		}
		return true;
	}
};