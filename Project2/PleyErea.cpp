#include "PleyErea.h"

PleyErea::PleyErea(Vector2&& size, Vector2&& offset)
{
	Init(std::move(size), std::move(offset));
}

PleyErea::~PleyErea()
{
}

void PleyErea::UpDate()
{
	_input->Update();
	_puyo->Update();
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, _size.x, _size.y, 0xff8888, true);
	_puyo->Draw();
}

void PleyErea::Init(Vector2&& size, Vector2&& offset)
{
	_size = std::move(size);
	_offset = std::move(offset);
	_input = std::make_unique<Input>();
	_screenID = MakeScreen(_size.x, _size.y, true);
	_puyo = new Puyo({ 0.0f,0.0f });
}

const Vector2 PleyErea::offset(void)
{
	return _offset;
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
