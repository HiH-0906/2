#include "keyState.h"

std::map<PLAYER_ID, keyData> keyState::_keyCon =
{
	{
		PLAYER_ID::ONE,
		keyData{
				keyPair{ INPUT_ID::LEFT,KEY_INPUT_A },
				keyPair{ INPUT_ID::RIGHT,KEY_INPUT_D },
				keyPair{ INPUT_ID::UP,KEY_INPUT_W },
				keyPair{ INPUT_ID::DOWN,KEY_INPUT_S },
				keyPair{INPUT_ID::ROAT,KEY_INPUT_Z}
				}
	},
	{
		PLAYER_ID::SECOND,
		keyData{
				keyPair{ INPUT_ID::LEFT,KEY_INPUT_LEFT },
				keyPair{ INPUT_ID::RIGHT,KEY_INPUT_RIGHT },
				keyPair{ INPUT_ID::UP,KEY_INPUT_UP },
				keyPair{ INPUT_ID::DOWN,KEY_INPUT_DOWN },
				keyPair{INPUT_ID::ROAT,KEY_INPUT_NUMPAD0}
				}
	},
};

void keyState::Update(const PLAYER_ID& p_id)
{
	for (auto id : _keyCon[p_id])
	{
		_input[id.first].second = _input[id.first].first;
		_input[id.first].first = CheckHitKey(id.second);
	}
}