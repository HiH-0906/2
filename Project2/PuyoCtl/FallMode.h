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
				puyo->SetMunyonBit(DirPermit{ 0 });
				stage.playErea_[vec.x][vec.y].reset();
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
						// �Ղ�[�񂵂Ȃ��Ղ悪�~���Ă��Ă����ꍇ���Ȃ�
						if (!stage.playErea_[vec.x][y]->SetPuyonCnt())
						{
							break;
						}
					}
					else
					{
						break;
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