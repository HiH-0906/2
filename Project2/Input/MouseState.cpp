#include <DxLib.h>
#include "MouseState.h"

MouseState::MouseState()
{
	_mouseData = 0;
	_mouseDataOld = 0;
}

MouseState::~MouseState()
{
}

CON_ID MouseState::GetID(void)
{
	return CON_ID::MOUSE;
}

void MouseState::Update(const int& p_id)
{
	_mouseDataOld = _mouseData;
	_mouseData = GetMouseInput();
	for (auto id : INPUT_ID())
	{
		_input[id].second = _input[id].first;
	}
	int mposx, mposy;
	GetMousePoint(&mposx, &mposy);
	// ‰„X‚Æ‚½‚Ü‚ébuff
	_buff = _stanPos - Vector2{ mposx,mposy };
	// ‰æ–Ê”¼•ª
	int tmp = 256;
	// ‚²‚è‰Ÿ‚µ
	if (_buff.x >= tmp)
	{
		_input[INPUT_ID::LEFT].first = true;
		_input[INPUT_ID::RIGHT].first = false;
	}
	else if (_buff.x <= -tmp)
	{
		_input[INPUT_ID::LEFT].first = false;
		_input[INPUT_ID::RIGHT].first = true;
	}
	else
	{
		_input[INPUT_ID::LEFT].first = false;
		_input[INPUT_ID::RIGHT].first = false;
	}
	if (_buff.y >= tmp)
	{
		_input[INPUT_ID::UP].first = true;
		_input[INPUT_ID::DOWN].first = false;
	}
	else if (_buff.y <= -tmp)
	{
		_input[INPUT_ID::UP].first = false;
		_input[INPUT_ID::DOWN].first = true;
	}
	else
	{
		_input[INPUT_ID::UP].first = false;
		_input[INPUT_ID::DOWN].first = false;
	}
	_input[INPUT_ID::LROTA].first = GetMouseTrg(MOUSE_INPUT_LEFT);
	_input[INPUT_ID::RROTA].first = GetMouseTrg(MOUSE_INPUT_RIGHT);
	if (GetMouseTrg(MOUSE_INPUT_MIDDLE))
	{
		// Î²°Ù¸Ø¯¸‚ÅŠî€ˆÊ’u•ÏX
		GetMousePoint(&_stanPos.x, &_stanPos.y);
	}
}

void MouseState::Setting(void)
{
	_buff = { 0, 0 };
	_stanPos = { 512,384 };
}
bool MouseState::GetMouseTrg(int id)
{
	return ((_mouseData & id) != 0 && (_mouseDataOld & id) == 0);
}
;
