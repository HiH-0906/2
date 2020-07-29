#pragma once
#include "../PleyErea.h"

struct EraseMode
{
	bool operator ()(PleyErea& stage)
	{
		// �����z�����邩�ǂ���
		bool delFlag = false;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			delFlag |= stage.SetErasePuyo(puyo->GetGrid(stage.blockSize_), puyo->id());
		});
		if (delFlag)
		{
			// �����z�����̂ŏ���
			stage.DeletePuyo();
			// ������x̫��
			stage.mode_ = STAGE_MODE::FALL;
			for (auto&& puyo : stage.puyoList_)
			{
				// �����鑬�xUP
				puyo->ChengeSpeed(PUYO_RAD / 2, 1);
			}
		}
		else
		{
			// �����z�����Ȃ��ꍇ�ނɂ�[���

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
				return false;
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
			stage.mode_ = STAGE_MODE::MUNYON;
		}
		return true;
	}
};