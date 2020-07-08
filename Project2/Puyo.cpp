#include <utility>
#include <DxLib.h>
#include "Puyo.h"

Puyo::Puyo(Vector2Flt&& pos, float&& rad)
{
	Init(std::move(pos), std::move(rad));
}

Puyo::~Puyo()
{
}

void Puyo::Update(void)
{
	_pos.y += _speed;
	if (_pos.y + _rad >= 576)
	{
		_pos.y = 0;
	}
}

void Puyo::Draw(void)
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), _rad, 0x8888ff, true);
}

void Puyo::Move(const Vector2Flt& vec)
{
	_pos += vec;
	if (_pos.x - _rad < 0.0f)
	{
		_pos.x = _rad;
	}
	if (_pos.x + _rad > 288.0f)
	{
		_pos.x = 288.0f - _rad;
	}
}

void Puyo::Init(Vector2Flt&& pos, float&& rad)
{
	_pos = std::move(pos);
	_rad = std::move(rad);
}
