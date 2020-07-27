#include <utility>
#include <DxLib.h>
#include "SceneMng.h"
#include"_debug/_DebugConOut.h"
#include "Puyo.h"

puyoColor Puyo::colorList_ = {
	{PUYO_ID::RED,0xff8888},
	{PUYO_ID::BULE,0x8888ff},
	{PUYO_ID::GREEN,0x88ff88},
	{PUYO_ID::PURPLE,0xff88ff},
	{PUYO_ID::YELLOW,0xffff88},
};

Puyo::Puyo(Vector2&& pos, PUYO_ID id)
{
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
	dirpermit_.perbit = { 0,0,0,0 };
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
	if (puyonCnt_)
	{
		puyonCnt_--;
	}
	if (muyonCnt_)
	{
		muyonCnt_--;
	}
	return false;
}

void Puyo::Draw(void)
{
	//DrawCircle(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_), static_cast<int>(rad_), colorList_[id_], true);
	DrawOval(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_), static_cast<int>(rad_), static_cast<int>(rad_), colorList_[id_], true);
	if (munyonmit_.perbit.up)
	{
		DrawBox(pos_.x, pos_.y, 1+pos_.x + rad_ * 2, pos_.y + rad_, colorList_[id_], true);
	}
	if (munyonmit_.perbit.down)
	{
		DrawBox(pos_.x, pos_.y + rad_, 1+pos_.x + rad_ * 2, pos_.y + rad_ * 2, colorList_[id_], true);
	}
	if (munyonmit_.perbit.left)
	{
		DrawBox(pos_.x, pos_.y, pos_.x + rad_, pos_.y + rad_ * 2, colorList_[id_], true);
	}
	if (munyonmit_.perbit.right)
	{
		DrawBox(pos_.x + rad_, pos_.y, pos_.x + rad_ * 2, pos_.y + rad_ * 2, colorList_[id_], true);
	}
}

const Vector2& Puyo::pos(void)
{
	return pos_;
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

void Puyo::SetPuyonCnt(void)
{
	puyonCnt_ = puyonMax_;
}

void Puyo::SetMuyonCnt(void)
{
	muyonCnt_ = muyonMax_;
}

bool Puyo::CheckMuyonCnt(void)
{
	return muyonCnt_;
}

void Puyo::ChengeSpeed(void)
{
	/*vec_[INPUT_ID::DOWN] = Vector2{ 0,rad_ * 2 };*/
}

bool Puyo::activ(void)
{
	return activ_;
}

void Puyo::activ(bool flag)
{
	activ_ = flag;
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
	dirpermit_ = dirpermit;
	return true;
}
