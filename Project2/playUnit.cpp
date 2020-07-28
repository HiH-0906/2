#include "playUnit.h"
#include "PleyErea.h"
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"

playUnit::playUnit(PleyErea& erea):playErea_(erea)
{
	targetID = 0;
}

playUnit::~playUnit()
{
}

bool playUnit::Update(void)
{
	auto CheckPair = [&](int tar) {
		if (!playErea_.puyoList_[tar^1]->dirpermit().perbit.down)
		{
			playErea_.mode_ = STAGE_MODE::FALL;
			playErea_.puyoList_[tar]->ChengeSpeed(PUYO_RAD / 2, 1);
			return true;
		}
		return false;
	};
	auto CheckSpinPuyo = [&](Vector2 puyo1,Vector2 puyo2,bool rotaRight) 
	{
		auto move = playErea_.blockSize_;
		int offsetY = (playErea_.puyoList_[targetID]->pos().y % playErea_.blockSize_ != 0);
		if (!rotaRight)
		{
			move = -playErea_.blockSize_;
		}
		if (puyo1.y < puyo2.y)
		{
			if (playErea_.playErea_[(puyo2.x + move) / playErea_.blockSize_][puyo1.y / playErea_.blockSize_])
			{
				return;
			}
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo2.x + move,puyo1.y });
		}
		if (puyo1.y > puyo2.y)
		{
			if (playErea_.playErea_[(puyo2.x - move) / playErea_.blockSize_][puyo1.y / playErea_.blockSize_])
			{
				return;
			}
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo2.x - move,puyo1.y });
		}
		if (puyo1.x < puyo2.x)
		{
			if (playErea_.playErea_[puyo2.x / playErea_.blockSize_][(puyo2.y - move) / playErea_.blockSize_ + offsetY])
			{
				return;
			}
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo1.x,puyo2.y - move });
		}
		if (puyo1.x > puyo2.x)
		{
			if (playErea_.playErea_[puyo2.x / playErea_.blockSize_][(puyo2.y + move) / playErea_.blockSize_ + offsetY])
			{
				return;
			}
			playErea_.puyoList_[targetID ^ 1]->pos(Vector2{ puyo1.x ,puyo2.y + move });
		}
		if (playErea_.puyoList_[0]->pos().y > playErea_.puyoList_[1]->pos().y)
		{
			std::swap(*playErea_.puyoList_[targetID], *playErea_.puyoList_[targetID ^ 1]);
			targetID ^= 1;
			TRACE("%d\n", targetID);
		}
	};
	auto SetPairMit = [&]()
	{
		auto vec1 = playErea_.puyoList_[targetID]->pos();
		auto vec2 = playErea_.puyoList_[targetID ^ 1]->pos();

		auto vec3 = vec2 - vec1;
		DirPermit mit;
		mit.per = 0;
		if (vec3.x < 0)
		{
			mit.perbit = { 0,0,1,0 };
			playErea_.puyoList_[targetID]->pairMit(mit);
			mit.perbit = { 0,0,0,1 };
			playErea_.puyoList_[targetID ^ 1]->pairMit(mit);
		}
		if (vec3.x > 0)
		{
			mit.perbit = { 0,0,0,1 };
			playErea_.puyoList_[targetID]->pairMit(mit);
			mit.perbit = { 0,0,1,0 };
			playErea_.puyoList_[targetID ^ 1]->pairMit(mit);
		}
		if (vec3.y < 0)
		{
			mit.perbit = { 1,0,0,0 };
			playErea_.puyoList_[targetID]->pairMit(mit);
			mit.perbit = { 0,1,0,0 };
			playErea_.puyoList_[targetID ^ 1]->pairMit(mit);
		}
		if (vec3.y > 0)
		{
			mit.perbit = { 0,1,0,0 };
			playErea_.puyoList_[targetID]->pairMit(mit);
			mit.perbit = { 1,0,0,0 };
			playErea_.puyoList_[targetID ^ 1]->pairMit(mit);
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
	if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::LROTA) || (*playErea_.input_)->GetKeyTrg(INPUT_ID::RROTA))
	{
		CheckSpinPuyo(playErea_.puyoList_[targetID]->pos(), playErea_.puyoList_[targetID^1]->pos(), (*playErea_.input_)->GetKeyTrg(INPUT_ID::RROTA));
	}
	SetPairMit();
	if (CheckPair(0) || CheckPair(1))
	{
		targetID = 0;
		return true;
	}
	return true;
}
