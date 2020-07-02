#pragma once
#include "Myself.h"
#include "TiketMachine.h"
#include "InsertCard.h"
#include "InsertCash.h"

struct InsertMax
{
	void operator()(Wallet wallet, MapInt& data)
	{
		if (wallet.payType == PayType::CASH)
		{
			InsertCash insert;
			lpTiketMachine.payType(PayType::CASH);
			lpMyself.SetInsert(InsertCash());
			insert(wallet, data);
		}
		else if (wallet.payType == PayType::CARD)
		{
			InsertCard insert;
			lpTiketMachine.payType(PayType::CARD);
			lpMyself.SetInsert(InsertCard());
			insert(wallet, data);
		}
		else
		{
			// Max‚©•Ï‚È’l‚ª—ˆ‚Ä‚à‰½‚à‚¹‚¸return
			return;
		}
	}
};
