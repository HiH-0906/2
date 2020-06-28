#pragma once
#include <utility>
#include <memory>

#define lpCardServer CardServer::GetInstance()

using PairInt = std::pair<int, int>;

class CardServer
{
public:
	static CardServer &GetInstance()
	{
		return *S_Instance;
	}
	PairInt GetCardState(void);					// ¶°ÄŞ‚Ìó‘Ôæ“¾
	bool PayMent(int price);
	bool MergeCard(PairInt card);
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
	PairInt _cardData;
	static std::unique_ptr<CardServer, CardServerDeleter> S_Instance;
};

