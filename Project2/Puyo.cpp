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

Puyo::Puyo(Vector2&& pos, int&& rad)
{
	pos_ = pos;
	rad_ = rad;
	activ_ = true;
	id_ = static_cast<PUYO_ID>((rand()%5)+1);
	downCnt_ = 0;
	downNum_ = 15;
	dirpermit_.perbit = { 0,0,0,0 };
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
	return false;
}

void Puyo::Draw(void)
{
	DrawCircle(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_), static_cast<int>(rad_), colorList_[id_], true);
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

bool Puyo::dirpermit(DirPermit dirpermit)
{
	dirpermit_ = dirpermit;
	return true;
}
