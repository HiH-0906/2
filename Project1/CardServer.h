#pragma once
#include <utility>
#include <memory>
#include "TiketMachine.h"

#define lpCardServer CardServer::GetInstance()



class CardServer
{
public:
	static CardServer &GetInstance()
	{
		return *S_Instance;
	}
	MapInt GetCardState(void);					// ¶°ÄŞ‚Ìó‘Ôæ“¾
	bool PayMent(int price);
	bool MergeCard(MapInt card);
private:
	struct CardServerDeleter
	{
		void operator()(CardServer* cardServer)const
		{
			delete cardServer;
		}
	};
	CardServer();
	~CardServer();
	MapInt _cardData;
	static std::unique_ptr<CardServer, CardServerDeleter> S_Instance;
};

