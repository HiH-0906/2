#include "keyState.h"

// ∑∞∫›Ã®∏ﬁê›íË í∑Ç¢
std::map<int, keyData> keyState::_keyCon =
{
	{
		0,
		keyData{
				keyPair{ INPUT_ID::LEFT,KEY_INPUT_A },
				keyPair{ INPUT_ID::RIGHT,KEY_INPUT_D },
				keyPair{ INPUT_ID::UP,KEY_INPUT_W },
				keyPair{ INPUT_ID::DOWN,KEY_INPUT_S },
				keyPair{INPUT_ID::LROTA,KEY_INPUT_C},
				keyPair{INPUT_ID::RROTA,KEY_INPUT_V},
				keyPair{INPUT_ID::POSE,KEY_INPUT_T}
				}
	},
	{
		1,
		keyData{
				keyPair{ INPUT_ID::LEFT,KEY_INPUT_LEFT },
				keyPair{ INPUT_ID::RIGHT,KEY_INPUT_RIGHT },
				keyPair{ INPUT_ID::UP,KEY_INPUT_UP },
				keyPair{ INPUT_ID::DOWN,KEY_INPUT_DOWN },
				keyPair{INPUT_ID::LROTA,KEY_INPUT_NUMPAD1},
				keyPair{INPUT_ID::RROTA,KEY_INPUT_NUMPAD2},
				keyPair{INPUT_ID::POSE,KEY_INPUT_NUMPAD9}
				}
	},
};

CON_ID keyState::GetID(void)
{
	return CON_ID::KEY;
}

void keyState::Update(void)
{
	for (auto id : _keyCon[p_id_])
	{
		input_[id.first].second = input_[id.first].first;
		input_[id.first].first = CheckHitKey(id.second);
	}
}