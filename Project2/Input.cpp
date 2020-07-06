#include "Input.h"

Input::Input()
{
	Init();
}

Input::~Input()
{
}

void Input::Update(void)
{
	_input[INPUT_ID::LEFT].second = _input[INPUT_ID::LEFT].first;
	_input[INPUT_ID::RIGHT].second = _input[INPUT_ID::RIGHT].first;
	_input[INPUT_ID::LEFT].first = CheckHitKey(static_cast<int>(INPUT_ID::LEFT));
	_input[INPUT_ID::RIGHT].first = CheckHitKey(static_cast<int>(INPUT_ID::RIGHT));
}

bool Input::GetKeyTrg(INPUT_ID key)
{
	return _input[key].first && !_input[key].second;
}


void Input::Init(void)
{
	_input.try_emplace(INPUT_ID::LEFT, std::pair<bool, bool>{ false,false });
	_input.try_emplace(INPUT_ID::RIGHT, std::pair<bool, bool>{ false,false });
}
