#include "PleyErea.h"

PleyErea::PleyErea(Vector2&& size, Vector2&& offset,PLAYER_ID&& id)
{
	Init(std::move(size), std::move(offset),std::move(id));
}

PleyErea::~PleyErea()
{
}

void PleyErea::UpDate()
{
	_input->Update(_id);
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, _size.x, _size.y, 0xff8888, true);
}

void PleyErea::Init(Vector2&& size, Vector2&& offset,PLAYER_ID&& id)
{
	_size = std::move(size);
	_offset = std::move(offset);
	_id = std::move(id);
	_input = std::make_unique<Input>();
	_screenID = MakeScreen(_size.x, _size.y, true);
}

const Vector2 PleyErea::offset(void)
{
	return _offset;
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
