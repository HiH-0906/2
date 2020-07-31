#pragma once
#include "../PleyErea.h"

struct EraseMode
{
	bool operator ()(PleyErea& stage)
	{
		// Á‚·“z‚ª‚ ‚é‚©‚Ç‚¤‚©
		bool delFlag = false;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](PuyoUnit& puyo) {
			delFlag |= stage.SetErasePuyo(puyo->GetGrid(stage.blockSize_), puyo->id());
		});
		if (delFlag)
		{
			stage.rensaNum_++;
			// Á‚·“z‚¢‚½‚Ì‚ÅÁ‚·
			stage.DeletePuyo();
			// ‚à‚¤ˆê“xÌ«°Ù
			stage.mode_ = STAGE_MODE::FALL;
			for (auto&& puyo : stage.puyoList_)
			{
				// —‚¿‚é‘¬“xUP
				puyo->ChengeSpeed(PUYO_RAD / 2, 1);
			}
		}
		else
		{
			// ˜A½”‚ğ‘Šè‚Ö‚©‚Â‘Å‚¿Á‚µ
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
						stage.ozyamaCnt_ = 0;
					}
					lpSceneMng.AddRensaQue(RENSA_QUE{ stage.playerID_,abs(checkNum)});
				}
				stage.rensaNum_ = 0;
			}
			// Á‚·“z‚ª‚¢‚È‚¢ê‡ÄŞÛ¯Ìß‚Ö
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