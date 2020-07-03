#pragma once
#include "Myself.h"
#include "TiketMachine.h"
#include "InsertCard.h"
#include "InsertCash.h"

struct InsertMax
{
	bool operator()(Wallet wallet, MapInt& data)
	{
		if (wallet.payType == PayType::CASH)
		{
			InsertCash insert;
			lpTiketMachine.payType(PayType::CASH);
			lpMyself.SetInsert(InsertCash());
			insert(wallet, data);
			return true;
		}
		else if (wallet.payType == PayType::CARD)
		{
			InsertCard insert;
			lpTiketMachine.payType(PayType::CARD);
			lpMyself.SetInsert(InsertCard());
			insert(wallet, data);
			return false;
		}
		else
		{
			// Max���ςȒl�����Ă���������return
			return false;
		}
		return false;
	}
};
