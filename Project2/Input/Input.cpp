#include <DxLib.h>
#include "Input.h"


Input::Input()
{
	Init();
}


void Input::Setting(void)
{
}

bool Input::GetKeyTrg(INPUT_ID key)
{
	if (key == INPUT_ID::MAX)
	{
		return false;
	}
	return _input[key].first && !_input[key].second;
}

bool Input::GetKeySty(INPUT_ID key)
{
	if (key == INPUT_ID::MAX)
	{
		return false;
	}
	return _input[key].first && _input[key].second;
}

void Input::SetDownTrg(void)
{
	_input[INPUT_ID::DOWN].first = true;
	_input[INPUT_ID::DOWN].second = false;
}


void Input::Init()
{
	for (auto id : INPUT_ID())
	{
		_input.try_emplace(id, std::pair<bool, bool>{ false, false });
	}
}
