#include "Obj.h"

Obj::Obj()
{
	Init();
}

Obj::~Obj()
{
}

const float Obj::rad()const
{
	return _rad;
}

void Obj::Init(void)
{
	_pos = { 0.0f,0.0f };
	_rad = 48.0f;
	_speed = (_rad * 2.0f) / 60.0f;
	_alive = true;
}
