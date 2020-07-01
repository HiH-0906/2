#pragma once
#include "TiketMachine.h"

struct InsertCash
{
	void operator()(Wallet wallet)
	{
		if (lpMyself.cash()[wallet.cash] <= 0)
		{
			return;
		}
		lpTiketMachine.cashData().try_emplace(wallet.cash, 0);
		lpTiketMachine.cashData()[wallet.cash]++;
		lpMyself.cash()[wallet.cash]--;
	}
};
