#include "playUnit.h"
#include "PleyErea.h"
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"

playUnit::playUnit(PleyErea& erea):playErea_(erea)
{
	Init();
}

playUnit::~playUnit()
{
}

bool playUnit::Update(void)
{
	// ���������n���Ă��Ȃ�������
	auto CheckPair = [&](int tar) {
		if (!playErea_.puyoList_[tar^1]->dirpermit().perbit.down)
		{
			// ���n���Ă����痎�����邾��
			playErea_.mode_ = STAGE_MODE::FALL;
			playErea_.puyoList_[tar]->ChengeSpeed(PUYO_RAD / 2, 1);
			return true;
		}
		return false;
	};
	if (playErea_.mode_ != STAGE_MODE::DROP)
	{
		return false;
	}
	for (auto id : INPUT_ID())
	{
		// �e���͗p�֐��Ăяo��
		keyFunc_[id]();
	}
	if (CheckPair(0) || CheckPair(1))
	{
		// ���n�����ޯ�ؾ��
		targetID_ = 0;
		return true;
	}
	// ���삵�Ă��邺��
	playErea_.puyoList_[targetID_]->playPuyo(true);
	return true;
}

void playUnit::Init(void)
{
	targetID_ = 0;
	keyFunc_.try_emplace(INPUT_ID::UP, [&]() {
		// �Ȃ�����Ȃ�
	});
	// �e���͗p�֐��o�^ �Ȃ񂩂܂Ƃ߂ꂻ��������
	keyFunc_.try_emplace(INPUT_ID::DOWN, [&]() {
		if ((*playErea_.input_)->GetKeySty(INPUT_ID::DOWN))
		{
			playErea_.puyoList_[0]->SetSoftDrop();
			playErea_.puyoList_[1]->SetSoftDrop();
		}
	});
	keyFunc_.try_emplace(INPUT_ID::LEFT, [&]() {

		if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::LEFT))
		{
			auto vec1 = playErea_.puyoList_[0]->GetGrid(playErea_.blockSize_);
			auto vec2 = playErea_.puyoList_[1]->GetGrid(playErea_.blockSize_);
			int offsetY = (playErea_.puyoList_[0]->pos().y % playErea_.blockSize_ != 0);
			int offsetY2 = (playErea_.puyoList_[1]->pos().y % playErea_.blockSize_ != 0);
			if (!playErea_.playErea_[vec1.x - 1][vec1.y+ offsetY] && !playErea_.playErea_[vec2.x - 1][vec2.y+ offsetY2])
			{
				playErea_.puyoList_[0]->Move(INPUT_ID::LEFT);
				playErea_.puyoList_[1]->Move(INPUT_ID::LEFT);
			}
		}
	});
	keyFunc_.try_emplace(INPUT_ID::RIGHT, [&]() {

		if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::RIGHT))
		{
			auto vec1 = playErea_.puyoList_[0]->GetGrid(playErea_.blockSize_);
			auto vec2 = playErea_.puyoList_[1]->GetGrid(playErea_.blockSize_);
			int offsetY = (playErea_.puyoList_[0]->pos().y % playErea_.blockSize_ != 0);
			int offsetY2 = (playErea_.puyoList_[1]->pos().y % playErea_.blockSize_ != 0);
			if (!playErea_.playErea_[vec1.x + 1][vec1.y+ offsetY] && !playErea_.playErea_[vec2.x + 1][vec2.y+ offsetY2])
			{
				playErea_.puyoList_[0]->Move(INPUT_ID::RIGHT);
				playErea_.puyoList_[1]->Move(INPUT_ID::RIGHT);
			}
		}
	});
	keyFunc_.try_emplace(INPUT_ID::RROTA, [&]() {
		if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::RROTA))
		{
			RotaPuyo(playErea_.puyoList_[targetID_]->pos(), playErea_.puyoList_[targetID_ ^ 1]->pos(), true);
		}
	});
	keyFunc_.try_emplace(INPUT_ID::LROTA, [&]() {
		if ((*playErea_.input_)->GetKeyTrg(INPUT_ID::LROTA))
		{
			RotaPuyo(playErea_.puyoList_[targetID_]->pos(), playErea_.puyoList_[targetID_ ^ 1]->pos(), false);
		}
	});
}

void playUnit::RotaPuyo(Vector2 puyo1, Vector2 puyo2, bool rotaRight)
{
	auto move = playErea_.blockSize_;
	int offsetY = (playErea_.puyoList_[targetID_]->pos().y % playErea_.blockSize_ != 0);
	int tmp = -playErea_.blockSize_;
	if (!rotaRight)
	{
		move = -playErea_.blockSize_;
		tmp = -tmp;
	}
	Vector2 rotaPos = { 0,0 };
	if (puyo1.y < puyo2.y)
	{
		rotaPos = Vector2{ puyo2.x + move,puyo1.y };
	}
	if (puyo1.y > puyo2.y)
	{
		rotaPos = Vector2{ puyo2.x - move,puyo1.y };
		tmp = -tmp;
	}
	if (puyo1.x < puyo2.x)
	{
		rotaPos = Vector2{ puyo1.x,puyo2.y - move };
	}
	if (puyo1.x > puyo2.x)
	{
		rotaPos = Vector2{ puyo1.x ,puyo2.y + move };
	}
	auto vec = playErea_.ConvertGrid(std::move(rotaPos));
	if (!playErea_.playErea_[vec.x][vec.y + offsetY])
	{
		playErea_.puyoList_[targetID_ ^ 1]->pos(std::move(rotaPos));
	}
	else
	{
		auto rotaPos2 = rotaPos;
		if (vec.y >= playErea_.stgSize_.y - 2)
		{
			rotaPos.y -= abs(tmp);
			rotaPos2.y -= abs(tmp * 2);
		}
		else
		{
			rotaPos.x += tmp;
			rotaPos2.x += tmp + tmp;
		}
		vec= playErea_.ConvertGrid(std::move(rotaPos));
		auto vec2 = playErea_.ConvertGrid(std::move(rotaPos2));
		if (!playErea_.playErea_[vec.x][vec.y + offsetY] && !playErea_.playErea_[vec2.x][vec2.y + offsetY])
		{
			playErea_.puyoList_[targetID_ ^ 1]->pos(std::move(rotaPos));
			playErea_.puyoList_[targetID_]->pos(std::move(rotaPos2));
		}
	}
	if (playErea_.puyoList_[0]->pos().y > playErea_.puyoList_[1]->pos().y)
	{
		std::swap(*playErea_.puyoList_[targetID_], *playErea_.puyoList_[targetID_ ^ 1]);
		targetID_ ^= 1;
		TRACE("%d\n", targetID_);
	}
}

const int playUnit::targetID(void) const
{
	return targetID_;
}
