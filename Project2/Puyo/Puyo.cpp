#include <utility>
#include <DxLib.h>
#include"../Scene/SceneMng.h"
#include"../_debug/_DebugConOut.h"
#include "Puyo.h"

puyoColor Puyo::colorList_ = {
	{PUYO_ID::RED,0xff8888},
	{PUYO_ID::BULE,0x8888ff},
	{PUYO_ID::GREEN,0x88ff88},
	{PUYO_ID::PURPLE,0xff88ff},
	{PUYO_ID::YELLOW,0xffff88},
	{PUYO_ID::OZAYMA,0x888888}
};

Puyo::Puyo()
{
	playPuyo_ = false;
	pos_ = {0,0};
	rad_ = PUYO_RAD;
	activ_ = true;
	id_ = PUYO_ID::RED;
	puyonCnt_ = 0;
	puyonMax_ = 12;
	muyonCnt_ = 0;
	muyonMax_ = 12;
	downCnt_ = 0;
	downNum_ = 15;
	dirpermit_.per = 0;
	oldDirpermit_.per = 0;
	munyonmit_.per = 0;
	vec_ = {
			{INPUT_ID::LEFT,Vector2{-rad_ * 2,0}},
			{INPUT_ID::RIGHT,Vector2{rad_ * 2,0}},
			{INPUT_ID::UP,Vector2{0,-rad_ * 2}},
			{INPUT_ID::DOWN,Vector2{0,rad_ / 2}}
	};
}

Puyo::Puyo(Vector2&& pos, PUYO_ID id)
{
	playPuyo_ = false;
	pos_ = pos;
	rad_ = PUYO_RAD;
	activ_ = true;
	id_ = id;
	puyonCnt_ = 0;
	puyonMax_ = 12;
	muyonCnt_ = 0;
	muyonMax_ = 12;
	downCnt_ = 0;
	downNum_ = 15;
	dirpermit_.per = 0;
	oldDirpermit_.per = 0;
	munyonmit_.per = 0;
	vec_ = {
			{INPUT_ID::LEFT,Vector2{-rad_ * 2,0}},
			{INPUT_ID::RIGHT,Vector2{rad_ * 2,0}},
			{INPUT_ID::UP,Vector2{0,-rad_ * 2}},
			{INPUT_ID::DOWN,Vector2{0,rad_ / 2}}
	};
}

Puyo::~Puyo()
{
}

bool Puyo::Update(void)
{
	if (puyonCnt_)
	{
		puyonCnt_--;
	}
	if (muyonCnt_)
	{
		muyonCnt_--;
	}
	if (downCnt_ < downNum_)
	{
		downCnt_++;
		return false;
	}
	if (dirpermit_.perbit.down)
	{
		pos_.y += vec_[INPUT_ID::DOWN].y;
		downCnt_ = 0;
		return false;
	}
	else
	{
		return true;
	}
	return false;
}

void Puyo::Draw(int cnt)
{
	auto puyon = abs(abs(puyonCnt_ - 6) - 6);
	if (playPuyo_)
	{
		int tmpRad = rad_ + 2;
		DrawOval(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_ + puyon + ((puyon * 2) * cnt)), static_cast<int>(tmpRad), static_cast<int>(tmpRad - puyon), 0xffffff, true);
	}
	DrawOval(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_ + puyon + ((puyon * 2) * cnt)), static_cast<int>(rad_ + (puyon / 2)), static_cast<int>(rad_ - puyon), colorList_[id_], true);

	if (munyonmit_.perbit.up)
	{
		DrawBox(pos_.x - (puyon / 2), (pos_.y + puyon + ((puyon * 3) * cnt)), (1 + (pos_.x) + rad_ * 2) + (puyon / 2), (pos_.y + puyon + ((puyon * 2) * cnt)) + rad_, colorList_[id_], true);
	}
	if (munyonmit_.perbit.down)
	{
		DrawBox(pos_.x - (puyon / 2), (pos_.y + puyon + ((puyon * 3) * cnt)) + rad_, (1 + (pos_.x) + rad_ * 2) + (puyon / 2), (pos_.y + puyon + ((puyon * 2) * cnt)) + rad_ * 2, colorList_[id_], true);
	}
	if (munyonmit_.perbit.left)
	{
		DrawBox(pos_.x, pos_.y + (puyon * 2) + ((puyon * 2) * cnt), 1+pos_.x + rad_, pos_.y + rad_ * 2, colorList_[id_], true);
	}
	if (munyonmit_.perbit.right)
	{
		DrawBox(pos_.x + rad_, pos_.y + (puyon * 2) + ((puyon * 2) * cnt), 1 + pos_.x + rad_ * 2, pos_.y + rad_ * 2, colorList_[id_], true);
	}
}

const Vector2& Puyo::pos(void)
{
	return pos_;
}

void Puyo::pos(Vector2&& pos)
{
	pos_ = pos;
}

const Vector2 Puyo::GetGrid(int size)
{
	Vector2 grid = pos_ / size;
	return grid;
}

PUYO_ID& Puyo::id(void)
{
	return id_;
}

void Puyo::SetSoftDrop(void)
{
	downCnt_ = downNum_;
}

bool Puyo::SetPuyonCnt(void)
{
	puyonCnt_ = puyonMax_;
	return true;
}

void Puyo::SetMuyonCnt(void)
{
	muyonCnt_ = muyonMax_;
}

bool Puyo::CheckPuyonCnt(void)
{
	return puyonCnt_;
}

bool Puyo::CheckMuyonCnt(void)
{
	return muyonCnt_;
}

void Puyo::ChengeSpeed(int speed, int cnt)
{
	vec_[INPUT_ID::DOWN] = Vector2{ 0,speed };
	downNum_ = cnt;
}

bool Puyo::activ(void)
{
	return activ_;
}

void Puyo::activ(bool flag)
{
	activ_ = flag;
}

void Puyo::playPuyo(bool flag)
{
	playPuyo_ = flag;
}

bool Puyo::playPuyo(void)
{
	return playPuyo_;
}

int Puyo::GetColor(void)
{
	return colorList_[id_];
}

void Puyo::Move(const INPUT_ID& id)
{
	auto tmpPos = pos_;
	switch (id)
	{
	case INPUT_ID::LEFT:
		if (dirpermit_.perbit.left)
		{
			tmpPos += vec_[id];
		}
		break;
	case INPUT_ID::RIGHT:
		if (dirpermit_.perbit.right)
		{
			tmpPos += vec_[id];
		}
		break;
	case INPUT_ID::UP:
		if (dirpermit_.perbit.up)
		{
			tmpPos += vec_[id];
		}
		break;
	case INPUT_ID::DOWN:
		if (dirpermit_.perbit.down)
		{
			SetSoftDrop();
		}
		break;
	case INPUT_ID::LROTA:
		break;
	case INPUT_ID::RROTA:
		break;
	case INPUT_ID::MAX:
		break;
	default:
		break;
	}
	pos_ = tmpPos;
}

void Puyo::SetMunyonBit(DirPermit dirpermit)
{
	munyonmit_ = dirpermit;
}

bool Puyo::dirpermit(DirPermit dirpermit)
{
	oldDirpermit_ = dirpermit_;
	dirpermit_ = dirpermit;
	return true;
}

bool Puyo::GetDownNow(void)
{
	return (dirpermit_.perbit.down == 0 && oldDirpermit_.perbit.down == 1);
}

const DirPermit Puyo::dirpermit(void)
{
	return dirpermit_;
}
