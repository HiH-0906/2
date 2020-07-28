#include "playUnit.h"
#include "PleyErea.h"

playUnit::playUnit(PleyErea& erea):playErea_(erea)
{
	targetID = 0;
}

playUnit::~playUnit()
{
}

bool playUnit::Update(void)
{
	auto CheckSpinPuyo = [&](Vector2 puyo1,Vector2 puyo2,bool rotaRight) 
	{
		auto move = playErea_.blockSize_;
		if (!rotaRight)
		{
			move = -playErea_.blockSize_;
		}
		if (puyo1.y < puyo2.y)
		{
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo2.x + move,puyo1.y });
		}
		if (puyo1.y > puyo2.y)
		{
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo2.x - move,puyo1.y });
		}
		if (puyo1.x < puyo2.x)
		{
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo1.x,puyo2.y + move });
		}
		if (puyo1.x > puyo2.x)
		{
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo1.x ,puyo2.y - move });
		}
		if (puyo1.y > puyo2.y)
		{
			std::swap(*playErea_.puyoList_[targetID], *playErea_.puyoList_[targetID ^ 1]);
			targetID ^= 1;
		}
	};
	if (playErea_.mode_ != STAGE_MODE::DROP)
	{
		return false;
	}
	if ((*playErea_.input_)->GetKeySty(INPUT_ID::DOWN))
	{
		playErea_.puyoList_[0]->SetSoftDrop();
		playErea_.puyoList_[1]->SetSoftDrop();
	}
	for (auto id : INPUT_ID())
	{
		if ((*playErea_.input_)->GetKeyTrg(id))
		{
			playErea_.puyoList_[0]->Move(id);
			playErea_.puyoList_[1]->Move(id);
		}
	}
	if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::LROTA)|| (*playErea_.input_)->GetKeyTrg(INPUT_ID::RROTA))
	{
		CheckSpinPuyo(playErea_.puyoList_[targetID]->pos(), playErea_.puyoList_[targetID^1]->pos(), (*playErea_.input_)->GetKeyTrg(INPUT_ID::RROTA));
	}
	return true;
}
