#pragma once
#include "../PleyErea.h"

struct PuyonMode
{
	int operator ()(PleyErea& stage)
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
			// �ނɂ�[��BitSet�֐�
			auto SetBit = [&](PUYO_ID id, Vector2 vec)
			{
				if (!stage.playErea_[vec.x][vec.y])
				{
					return false;
				}
				// �Ȃ����Ă������H
				return stage.playErea_[vec.x][vec.y]->id() == id;
			};
			// �ްѵ��ް�ʒu����
			if (stage.playErea_[4][1])
			{
				return -1;
			}
			// �ނ�[������
			for (auto&& puyo : stage.puyoList_)
			{
				DirPermit bit;
				auto vec = puyo->GetGrid(stage.blockSize_);
				auto id = puyo->id();
				bit.perbit.up = SetBit(id, { vec.x,vec.y - 1 });
				bit.perbit.down = SetBit(id, { vec.x,vec.y + 1 });
				bit.perbit.left = SetBit(id, { vec.x - 1,vec.y });
				bit.perbit.right = SetBit(id, { vec.x + 1,vec.y });
				puyo->SetMunyonBit(bit);
				puyo->SetMuyonCnt();
			}
			// �ނ�[��
			stage.mode_ = STAGE_MODE::MUNYON;
		}
		return 0;
	}
};