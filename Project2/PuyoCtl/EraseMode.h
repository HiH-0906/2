#pragma once
#include <DxLib.h>
#include "../PleyErea.h"
#include "../SoundMng.h"

struct EraseMode
{
	int operator ()(PleyErea& stage)
	{
		int reNum = 0;
		// Á‚·“z‚ª‚ ‚é‚©‚Ç‚¤‚©
		bool delFlag = false;
		std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
			delFlag |= stage.SetErasePuyo(puyo->GetGrid(stage.blockSize_), puyo->id());
		});
		if (delFlag)
		{
			stage.rensaNum_++;
			PlaySoundMem(lpSoundMng.GetHandle("–A"), DX_PLAYTYPE_BACK);
			// Á‚·“z‚¢‚½‚Ì‚ÅÁ‚·
			stage.DeletePuyo();
			std::for_each(stage.puyoList_.rbegin(), stage.puyoList_.rend(), [&](sharPuyo& puyo) {
				if (!stage.CheckMovePuyo(puyo))
				{
					// ‘‚«‚ñ‚Å‚ ‚éî•ñíœ
					auto vec = puyo->GetGrid(stage.blockSize_);
					puyo->SetMunyonBit(DirPermit{ 0 });
					stage.playErea_[vec.x][vec.y].reset();
				}
			});
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
				if (stage.rensaNum_ > stage.rensaMax_)
				{
					stage.rensaMax_ = stage.rensaNum_;
				}
				// ‚¨×–‚ŒvZ ŠÖ”‚É‚µ‚½•û‚ª‚¢‚¢‚©‚à

				// ‰Î—Í‚ª‚È‚¢
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
					//lpSceneMng.AddRensaQue(RENSA_QUE{ stage.playerID_,abs(checkNum),stage.eraseCnt_ });
					reNum = abs(checkNum);
				}
				stage.rensaNum_ = 0;
				stage.eraseCnt_ = 0;
			}
			// Á‚·“z‚ª‚¢‚È‚¢ê‡ÄŞÛ¯Ìß‚Ö
			if (stage.ozyamaCnt_)
			{
				stage.mode_ = STAGE_MODE::OZYAMA;
				return reNum;
			}
			stage.InstancePuyo();
			stage.CheckMovePuyo(stage.puyoList_[0]);
			stage.mode_ = STAGE_MODE::DROP;
		}
		return reNum;
	}
};