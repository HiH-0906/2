#pragma once
#include "../PleyErea.h"

struct EraseMode
{
	bool operator ()(PleyErea& stage)
	{
		// 消す奴があるかどうか
		bool delFlag = false;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			delFlag |= stage.SetErasePuyo(puyo->GetGrid(stage.blockSize_), puyo->id());
		});
		if (delFlag)
		{
			stage.rensaNum_++;
			// 消す奴いたので消す
			stage.DeletePuyo();
			// もう一度ﾌｫｰﾙ
			stage.mode_ = STAGE_MODE::FALL;
			for (auto&& puyo : stage.puyoList_)
			{
				// 落ちる速度UP
				puyo->ChengeSpeed(PUYO_RAD / 2, 1);
			}
		}
		else
		{
			// 連鎖数を相手へかつ打ち消し
			if (stage.rensaNum_)
			{
				auto checkNum = stage.ozyamaCnt_ - stage.rensaNum_ * stage.rensaNum_;
				if (checkNum >= 0 )
				{
					stage.ozyamaCnt_ = checkNum;
				}
				else
				{
					if (stage.ozyamaCnt_ != 0)
					{
						stage.ozyamaCnt_ += abs(checkNum);
					}
					lpSceneMng.AddRensaQue(RENSA_QUE{ stage.playerID_,stage.rensaNum_ });
				}
				stage.rensaNum_ = 0;
			}
			// 消す奴がいない場合ﾄﾞﾛｯﾌﾟへ

			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
		return true;
	}
};