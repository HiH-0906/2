#include <DxLib.h>
#include "Input.h"

std::map<PLAYER_ID, keyData> Input::_keyCon =
{
	{
		PLAYER_ID::ONE, keyData{keyPair{ INPUT_ID::LEFT,KEY_INPUT_A },keyPair{ INPUT_ID::RIGHT,KEY_INPUT_D },
		keyPair{ INPUT_ID::UP,KEY_INPUT_W }, keyPair{ INPUT_ID::DOWN,KEY_INPUT_S }}
	},
	{
		PLAYER_ID::SECOND, keyData{keyPair{ INPUT_ID::LEFT,KEY_INPUT_LEFT },keyPair{ INPUT_ID::RIGHT,KEY_INPUT_RIGHT },
		keyPair{ INPUT_ID::UP,KEY_INPUT_UP }, keyPair{ INPUT_ID::DOWN,KEY_INPUT_DOWN }}
	},
};

Input::Input()
{
	Init();
}

Input::~Input()
{
}

void Input::Update(const PLAYER_ID& p_id)
{
	for (auto id:_keyCon[p_id])
	{
		_input[id.first].second = _input[id.first].first;
		_input[id.first].first = CheckHitKey(id.second);
	}
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
