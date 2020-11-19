#include "PadState.h"
#include "../_debug/_DebugConOut.h"

std::map<int, keyData> PadState::_keyCon =
{
	{
		0,
		keyData{
		keyPair{ INPUT_ID::LEFT,PAD_INPUT_LEFT },
		keyPair{ INPUT_ID::RIGHT,PAD_INPUT_RIGHT },
		keyPair{ INPUT_ID::UP,PAD_INPUT_UP },
		keyPair{ INPUT_ID::DOWN,PAD_INPUT_DOWN },
		keyPair{ INPUT_ID::LROTA,PAD_INPUT_1},
		keyPair{ INPUT_ID::RROTA,PAD_INPUT_2},
		keyPair{INPUT_ID::POSE,PAD_INPUT_8}
		}
	}
	,
	{
		1,
		keyData{
		keyPair{ INPUT_ID::LEFT,PAD_INPUT_LEFT },
		keyPair{ INPUT_ID::RIGHT,PAD_INPUT_RIGHT },
		keyPair{ INPUT_ID::UP,PAD_INPUT_UP },
		keyPair{ INPUT_ID::DOWN,PAD_INPUT_DOWN },
		keyPair{ INPUT_ID::LROTA,PAD_INPUT_1},
		keyPair{ INPUT_ID::RROTA,PAD_INPUT_2},
		keyPair{INPUT_ID::POSE,PAD_INPUT_8}
		}
	}
};

CON_ID PadState::GetID(void)
{
	return CON_ID::PAD;
}

void PadState::Update(void)
{
	// DxLibPower
	for (auto id : _keyCon[0])
	{
		input_[id.first].second = input_[id.first].first;
		input_[id.first].first = (GetJoypadInputState(DX_INPUT_PAD1) & id.second);
	}
}

void PadState::Setting(const int& p_id, const int& pad_id)
{
	SetUseJoypadVibrationFlag(false);
	SetJoypadDeadZone(DX_INPUT_PAD1, 0.8);
	p_id_ = p_id;
	pad_id_ = pad_id;
}
