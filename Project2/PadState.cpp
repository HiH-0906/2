#include "PadState.h"
#include "_debug/_DebugConOut.h"

keyData PadState::_keyCon =
{
	keyPair{ INPUT_ID::LEFT,PAD_INPUT_LEFT },
	keyPair{ INPUT_ID::RIGHT,PAD_INPUT_RIGHT },
	keyPair{ INPUT_ID::UP,PAD_INPUT_UP },
	keyPair{ INPUT_ID::DOWN,PAD_INPUT_DOWN },
	keyPair{ INPUT_ID::ROAT,PAD_INPUT_1}
};

void PadState::Update(const PLAYER_ID& p_id)
{
	for (auto id : _keyCon)
	{
		_input[id.first].second = _input[id.first].first;
		_input[id.first].first = (GetJoypadInputState(static_cast<int>(p_id)+1)& id.second);
	}
}
