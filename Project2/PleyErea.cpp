#include "PleyErea.h"
#include "SceneMng.h"
#include "Input/keyState.h"
#include "Input/PadState.h"

int PleyErea::_allStage = 0;

PleyErea::PleyErea(Vector2&& size):_size(size)
{
	_playerID = static_cast<PLAYER_ID>(_allStage);
	_allStage++;
	_input = std::make_shared<Input*>(new PadState());
	_screenID = MakeScreen(_size.x, _size.y, true);
	_puyo = std::make_shared<Puyo>(Vector2Flt{ 24.0f,24.0f }, 24.0f);
}

PleyErea::~PleyErea()
{
	_allStage--;
}

void PleyErea::UpDate()
{
	(*_input)->Update(_playerID);

	auto move = [](std::weak_ptr<Input*> input,PLAYER_ID P_id,INPUT_ID IN_id, std::weak_ptr<Puyo> puyo,const Vector2Flt vec)
	{
		if (!input.expired())
		{
			if ((*input.lock())->GetKeyTrg(P_id, IN_id))
			{
				puyo.lock()->Move(vec);
			}
		}
	};
	move(_input, _playerID, INPUT_ID::LEFT, _puyo, Vector2Flt{ -_puyo->rad() * 2.0f ,0.0f });
	move(_input, _playerID, INPUT_ID::RIGHT, _puyo, Vector2Flt{ _puyo->rad() * 2.0f ,0.0f });

	_puyo->Update();
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	auto color = 0x000066 << (16 * static_cast<int>(_playerID));
	DrawBox(0, 0, _size.x, _size.y, color, true);
	_puyo->Draw();
}

const Vector2 PleyErea::offset(void)
{
	return Vector2{ lpSceneMng.screenSize().x / _allStage * static_cast<int>(_playerID),0 };
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
