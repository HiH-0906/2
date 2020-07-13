#include <utility>
#include <DxLib.h>
#include "Puyo.h"

Puyo::Puyo(Vector2Flt&& pos, float&& rad)
{
	_pos = pos;
	_rad = rad;
}

Puyo::~Puyo()
{
}

void Puyo::Update(void)
{
	/*_pos.y += _speed;
	if (_pos.y + _rad >= 512)
	{
		_pos.y = 0;
	}*/
}

void Puyo::Draw(void)
{
	DrawCircle(static_cast<int>(_pos.x), static_cast<int>(_pos.y), static_cast<int>(_rad), 0x8888ff, true);
}

void Puyo::Move(const Vector2Flt& vec)
{
	_pos += vec;
	if (_pos.x - _rad < 0.0f)
	{
		_pos.x = _rad;
	}
	if (_pos.x + _rad > 512.0f)
	{
		_pos.x = 512.0f - _rad;
	}
}
