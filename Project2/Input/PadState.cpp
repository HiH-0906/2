#include "PadState.h"
#include "../_debug/_DebugConOut.h"

keyData PadState::_keyCon =
{
	keyPair{ INPUT_ID::LEFT,PAD_INPUT_LEFT },
	keyPair{ INPUT_ID::RIGHT,PAD_INPUT_RIGHT },
	keyPair{ INPUT_ID::UP,PAD_INPUT_UP },
	keyPair{ INPUT_ID::DOWN,PAD_INPUT_DOWN },
	keyPair{ INPUT_ID::LROTA,PAD_INPUT_1},
	keyPair{ INPUT_ID::LROTA,PAD_INPUT_2}
};

CON_ID PadState::GetID(void)
{
	return CON_ID::PAD;
}

void PadState::Update(const int& p_id)
{
	// DxLibPower
	for (auto id : _keyCon)
	{
		_input[id.first].second = _input[id.first].first;
		_input[id.first].first = (GetJoypadInputState(/*static_cast<int>(p_id)+*/1)& id.second);
	}
}

void PadState::Setting(void)
{
	SetUseJoypadVibrationFlag(false);
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.8);
}
