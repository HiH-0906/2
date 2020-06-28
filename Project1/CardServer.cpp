#include "CardServer.h"

std::unique_ptr<CardServer, CardServer::CardServerDeleter> CardServer::S_Instance(new CardServer());

PairInt CardServer::GetCardState(void)
{
	return _cardData;
}

bool CardServer::PayMent(int price)
{
	if (_cardData.first >= price)
	{
		_cardData.first -= price;
		_cardData.second = price;
		return true;
	}
	return false;
}

bool CardServer::MergeCard(PairInt card)
{
	_cardData = card;
	return true;
}

CardServer::CardServer()
{
	_cardData = { 1000,0 };
}

CardServer::~CardServer()
{
}
