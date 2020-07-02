#pragma once
#include "TiketMachine.h"
#include "InsertCard2nd.h"
#include "CardServer.h"

struct InsertCard
{
	void operator()(Wallet wallet, MapInt& data)
	{
		data = lpCardServer.GetCardState();
		lpMyself.SetInsert(InsertCard2nd());
	}
};
