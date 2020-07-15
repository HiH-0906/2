#include <utility>
#include <DxLib.h>
#include "SceneMng.h"
#include"_debug/_DebugConOut.h"
#include "Puyo.h"

Puyo::Puyo(Vector2Flt&& pos, float&& rad)
{
	_pos = pos;
	_rad = rad;
	_dirpermit.perbit = { 0,0,0,0 };
	_vec = {
			{INPUT_ID::LEFT,Vector2Flt{-_rad * 2.0f,0.0f}} ,
			{INPUT_ID::RIGHT,Vector2Flt{_rad * 2.0f,0.0f}},
			{INPUT_ID::UP,Vector2Flt{0.0f,-_rad * 2.0f}},
			{INPUT_ID::DOWN,Vector2Flt{0.0f,_rad * 2.0f}}
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
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), static_cast<int>(_rad), 0x8888ff, true);
}

const Vector2Flt Puyo::pos(void)
{
	return _pos;
}

void Puyo::Move(const INPUT_ID& id)
{
	auto tmpPos = _pos;
	switch (id)
	{
	case INPUT_ID::LEFT:
		if (_dirpermit.perbit.left)
		{
			tmpPos += _vec[id];
		}
		break;
	case INPUT_ID::RIGHT:
		if (_dirpermit.perbit.right)
		{
			tmpPos += _vec[id];
		}
		break;
	case INPUT_ID::UP:
		if (_dirpermit.perbit.up)
		{
			tmpPos += _vec[id];
		}
		break;
	case INPUT_ID::DOWN:
		if (_dirpermit.perbit.down)
		{
			tmpPos += _vec[id];
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
	_pos = tmpPos;
}

bool Puyo::dirpermit(DirPermit dirpermit)
{
	_dirpermit = dirpermit;
	return true;
}
