#include "PleyErea.h"

PleyErea::PleyErea():_ereaSize(288,576)
{
	Init();
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

void PleyErea::Init(void)
{
	_input = std::make_unique<Input>();
	_screenID = MakeScreen(_ereaSize.x, _ereaSize.y, true);
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
