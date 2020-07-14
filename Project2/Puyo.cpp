#include <utility>
#include <DxLib.h>
#include "SceneMng.h"
#include"_debug/_DebugConOut.h"
#include "Puyo.h"

Puyo::Puyo(Vector2Flt&& pos, float&& rad)
{
	_pos = pos;
	_rad = rad;
	_moveflag.flag = 0;
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
	/*if (_pos.y + _speed >= PUYO_SIZE * STAGE_Y)
	{
		return;
	}
	_pos.y += _speed;*/
}

void Puyo::Draw(void)
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), static_cast<int>(_rad), 0x8888ff, true);
}

const Vector2Flt Puyo::GetMovePos(INPUT_ID id)
{
	auto tmpPos = _pos + _vec[id];
	return tmpPos;
}

void Puyo::Move(const INPUT_ID& id)
{
	auto tmpPos = _pos;
	tmpPos += _vec[id];
	_pos = tmpPos;
}
