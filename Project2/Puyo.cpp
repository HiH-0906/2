#include <utility>
#include <DxLib.h>
#include "SceneMng.h"
#include"_debug/_DebugConOut.h"
#include "Puyo.h"

Puyo::Puyo(Vector2&& pos, int&& rad)
{
	pos_ = pos;
	rad_ = rad;
	id_ = PUYO_ID::RED;
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

void Puyo::Update(void)
{
}

void Puyo::Draw(void)
{
	DrawCircle(static_cast<int>(pos_.x + rad_), static_cast<int>(pos_.y + rad_), static_cast<int>(rad_), 0x8888ff, true);
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
			tmpPos += vec_[id];
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
