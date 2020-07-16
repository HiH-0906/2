#include "Obj.h"

Obj::Obj()
{
	Init();
}

Obj::~Obj()
{
}

const int Obj::rad()const
{
	return _rad;
}

void Obj::Init(void)
{
	pos_ = { 0,0 };
	_rad = 48;
	_speed = (_rad * 2.0f) / 60.0f;
	alive_ = true;
}
