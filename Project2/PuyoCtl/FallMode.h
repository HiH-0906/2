#pragma once
#include "../PleyErea.h"

struct FallMode
{
	bool operator ()(PleyErea& stage)
	{
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			// �����Ă����[�H
			if (!stage.CheckMovePuyo(puyo))
			{
				// �����ꍇ��������ł�����폜
				auto vec = puyo->GetGrid(stage.blockSize_);
				stage.playErea_[vec.x][vec.y].reset();
			}
			auto SetBit = [&](PUYO_ID id, Vector2 vec)
			{
				if (id == PUYO_ID::OZAYMA)
				{
					return false;
				}
				if (!stage.playErea_[vec.x][vec.y])
				{
					return false;
				}
				// �Ȃ����Ă������H
				return stage.playErea_[vec.x][vec.y]->id() == id;
			};
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
		});
		bool eraseFlag = true;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			// �����Ă�������ꍇErase�Ɉڍs���Ȃ�
			if (!puyo->Update())
			{
				eraseFlag = false;
			}
		});
		if (eraseFlag)
		{
			// �����O�ɂՂ�[��
			stage.mode_ = STAGE_MODE::PUYON;
		}
		for (auto puyo : stage.puyoList_)
		{
			// �������Ă����Ղ悩�ǂ���
			if (!puyo->GetDownNow())
			{
				continue;
			}
			auto vec = puyo->GetGrid(stage.blockSize_);
			int cnt = 0;
			for (int y = vec.y; y < stage.stgSize_.y; y++)
			{
				if (stage.playErea_[vec.x][y])
				{
					// �ǂ͂Ղ�[�񂵂܂���
					if (stage.playErea_[vec.x][y]->id() != PUYO_ID::WALL)
					{
						// �Ղ�[��
						stage.playErea_[vec.x][y]->SetPuyonCnt();
					}
				}
				// �Ղ�[��͎����܂߂ĂR�l�܂łȂ񂾁c
				if (++cnt >= 3)
				{
					break;
				}
			}
		}
		return true;
	}
};