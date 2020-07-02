#include "CardServer.h"

std::unique_ptr<CardServer, CardServer::CardServerDeleter> CardServer::S_Instance(new CardServer());

MapInt CardServer::GetCardState(void)
{
	return _cardData;
}

bool CardServer::PayMent(int price)
{
	if (_cardData[static_cast<int>(CardType::BALANCE)] >= price)
	{
		_cardData[static_cast<int>(CardType::BALANCE)] -= price;
		_cardData[static_cast<int>(CardType::CHANGE)] = price;
		return true;
	}
	return false;
}

bool CardServer::MergeCard(MapInt card)
{
	_cardData = card;
	return true;
}

CardServer::CardServer()
{
	_cardData.try_emplace(static_cast<int>(CardType::BALANCE),1000);
	_cardData.try_emplace(static_cast<int>(CardType::CHANGE), 0);
}

CardServer::~CardServer()
{
}
