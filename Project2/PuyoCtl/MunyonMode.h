#pragma once
#include "../PleyErea.h"


struct MunyonMode
{
	bool operator ()(PleyErea& stage)
	{
		// ‚¿‚å‚Á‚Æ‚¾‚¯²İÀ°ÊŞÙ”­¶‚³‚¹‚Ä‚é
		bool muyon = false;
		for (auto puyo : stage.puyoList_)
		{
			// ‚Ş‚æ[‚ñ
			puyo->Update();
			muyon |= puyo->CheckMuyonCnt();
		}
		if (!muyon)
		{
			if (stage.ozyamaCnt_)
			{
				stage.mode_ = STAGE_MODE::OZYAMA;
				return true;
			}
			// ‚Ş‚æ[‚ñ¶³İÄŒãÁ‹
			stage.mode_ = STAGE_MODE::ERASE;
		}
		return true;
	}
};