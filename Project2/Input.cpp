#include <DxLib.h>
#include "Input.h"


Input::Input()
{
	Init();
}

Input::~Input()
{
}



bool Input::GetKeyTrg(PLAYER_ID& id,INPUT_ID& key)
{
	return _input[key].first && !_input[key].second;
}


void Input::Init()
{
	for (auto id : INPUT_ID())
	{
		_input.try_emplace(id, std::pair<bool, bool>{ false, false });
	}
}
