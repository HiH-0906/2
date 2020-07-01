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
			lpMyself._insert = InsertCash();
		}
		else if (wallet.payType == PayType::CARD)
		{
			lpTiketMachine.payType(PayType::CARD);
			lpMyself._insert = InsertCard();
		}
		else
		{
			// Max���ςȒl�����Ă��������Ȃ�
		}
		lpMyself._insert(wallet);
	}
};
