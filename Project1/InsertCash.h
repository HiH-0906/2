#pragma once
#include "TiketMachine.h"

struct InsertCash
{
	void operator()(Wallet wallet)
	{
		if (lpTiketMachine._paySuccess)
		{
			return;
		}
		if (lpMyself._cash[wallet.cash] <= 0)
		{
			return;
		}
		lpTiketMachine._cashData.try_emplace(wallet.cash, 0);
		lpTiketMachine._cashData[wallet.cash]++;
		lpMyself._cash[wallet.cash]--;
	}
};
