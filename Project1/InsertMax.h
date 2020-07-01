#pragma once
#include "Myself.h"
#include "TiketMachine.h"
#include "InsertCard.h"
#include "InsertCash.h"

struct InsertMax
{
	void operator()(Wallet wallet)
	{
		if (wallet.payType == PayType::CASH)
		{
			lpTiketMachine.payType(PayType::CASH);
			lpMyself.Insert(InsertCash());
		}
		else if (wallet.payType == PayType::CARD)
		{
			lpTiketMachine.payType(PayType::CARD);
			lpMyself.Insert(InsertCard());
		}
		else
		{
			// Maxか変な値が来ても何もしない
		}
		lpMyself.Insert()(wallet);
	}
};
