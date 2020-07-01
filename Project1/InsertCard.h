#pragma once
#include "TiketMachine.h"
#include "InsertCard2nd.h"

struct InsertCard
{
	void operator()(Wallet wallet)
	{
		lpTiketMachine.cardData(lpCardServer.GetCardState());
		lpMyself.Insert(InsertCard2nd());
	}
};
