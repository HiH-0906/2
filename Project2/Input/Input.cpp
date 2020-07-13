#include <DxLib.h>
#include "Input.h"


Input::Input()
{
	Init();
}


void Input::Setting(void)
{
}

bool Input::GetKeyTrg(INPUT_ID& key)
{
	if (key == INPUT_ID::MAX)
	{
		return false;
	}
	return _input[key].first && !_input[key].second;
}


void Input::Init()
{
	for (auto id : INPUT_ID())
	{
		_input.try_emplace(id, std::pair<bool, bool>{ false, false });
	}
}
