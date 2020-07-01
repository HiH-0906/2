#pragma once
#include "TiketMachine.h"
#include "InsertCard2nd.h"

struct InsertCard
{
	void operator()(Wallet wallet)
	{
		if (lpTiketMachine._paySuccess)
		{
			return;
		}
		lpTiketMachine._cardData = lpCardServer.GetCardState();
		lpMyself._insert = InsertCard2nd();
	}
};
