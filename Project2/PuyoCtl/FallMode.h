#pragma once
#include "../PleyErea.h"

struct FallMode
{
	int operator ()(PleyErea& stage)
	{
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
			// �����Ă����[�H
			stage.CheckMovePuyo(puyo);
		}
		);
		bool eraseFlag = true;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
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
		return 0;
	}
};