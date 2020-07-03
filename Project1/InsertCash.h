#pragma once
#include "TiketMachine.h"

struct InsertCash
{
	bool operator()(Wallet wallet, MapInt& data)
	{
		
		data.try_emplace(wallet.cash, 0);
		data[wallet.cash]++;
		return true;
	}
};
