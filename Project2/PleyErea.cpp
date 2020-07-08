#include "PleyErea.h"

PleyErea::PleyErea(Vector2 size, Vector2 offset)
{
	Init(size, offset);
}

PleyErea::~PleyErea()
{
}

void PleyErea::UpDate()
{
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, 288, 576, 0xff0000, true);
}

void PleyErea::Init(Vector2 size, Vector2 offset)
{
	_ereaSize = std::move(size);
	_offset = std::move(offset);
	_input = std::make_unique<Input>();
	_screenID = MakeScreen(_ereaSize.x, _ereaSize.y, true);
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
