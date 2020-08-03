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
			stage.rensaNum_++;
			// �����z�����̂ŏ���
			stage.DeletePuyo();
			std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
				if (!stage.CheckMovePuyo(puyo))
				{
					// ��������ł�����폜
					auto vec = puyo->GetGrid(stage.blockSize_);
					puyo->SetMunyonBit(DirPermit{ 0 });
					stage.playErea_[vec.x][vec.y].reset();
				}
			});
			stage.mode_ = STAGE_MODE::FALL;
			for (auto&& puyo : stage.puyoList_)
			{
				// �����鑬�xUP
				puyo->ChengeSpeed(PUYO_RAD / 2, 1);
			}
		}
		else
		{
			// �A�����𑊎�ւ��ł�����
			if (stage.rensaNum_)
			{
				if (stage.rensaNum_ > stage.rensaMax_)
				{
					stage.rensaMax_ = stage.rensaNum_;
				}
				// ���ז��v�Z �֐��ɂ���������������

				// �Η͂��Ȃ�
				// auto checkNum = stage.ozyamaCnt_ - (stage.rensaMax_ / 2) * (stage.rensaNum_ - 1) * (stage.eraseCnt_ / 4);
				stage.rensaNum_ = stage.rensaNum_ * (stage.rensaMax_ / 2);
				auto checkNum = stage.ozyamaCnt_ - (stage.rensaNum_ * stage.rensaNum_ + (stage.eraseCnt_ / 8));
				if (checkNum >= 0 )
				{
					stage.ozyamaCnt_ = checkNum;
				}
				else
				{
					if (stage.ozyamaCnt_ != 0)
					{
						stage.ozyamaCnt_ = 0;
					}
					lpSceneMng.AddRensaQue(RENSA_QUE{ stage.playerID_,abs(checkNum),stage.eraseCnt_ });
					
				}
				stage.rensaNum_ = 0;
				stage.eraseCnt_ = 0;
			}
			// �����z�����Ȃ��ꍇ��ۯ�߂�
			if (stage.ozyamaCnt_)
			{
				stage.mode_ = STAGE_MODE::OZYAMA;
				return true;
			}
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
		return true;
	}
};