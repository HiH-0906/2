#pragma once
#include "_debug/_DebugConOut.h"

struct InsertCard2nd
{
	bool operator()(Wallet wallet, MapInt& data)
	{
		TRACE("InsertCard2nd�ł�\n");
		return false;
	}
};