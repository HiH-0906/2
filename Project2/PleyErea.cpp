#include "PleyErea.h"
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "Input/keyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"

int PleyErea::_allStage = 0;

PleyErea::PleyErea(Vector2&& size,CON_ID id):_size(size)
{
	_playerID = _allStage;
	_allStage++;
	_color = 0x000066 << (16 * static_cast<int>(_playerID));
	_screenID = MakeScreen(_size.x, _size.y, true);
	_puyo = std::make_shared<Puyo>(Vector2Flt{ 24.0f,24.0f }, 24.0f);
	switch (id)
	{
	case CON_ID::KEY:
		_input = std::make_shared<Input*>(new keyState());
		break;
	case CON_ID::PAD:
		_input = std::make_shared<Input*>(new PadState());
		break;
	case CON_ID::MOUSE:
		_input = std::make_shared<Input*>(new MouseState());
		break;
	case CON_ID::MAX:
		_input = std::make_shared<Input*>(new keyState());
		break;
	default:
		TRACE("コントローラーがdefault");
		break;
	}
	(*_input)->Setting();
}

PleyErea::~PleyErea()
{
	_allStage--;
}

void PleyErea::UpDate()
{
	(*_input)->Update(_playerID);

	auto move = [](std::weak_ptr<Input*> input,INPUT_ID IN_id, std::weak_ptr<Puyo> puyo,const Vector2Flt vec)
	{
		if (!input.expired())
		{
			if ((*input.lock())->GetKeyTrg(IN_id))
			{
				puyo.lock()->Move(vec);
			}
		}
	};
	move(_input, INPUT_ID::LEFT, _puyo, Vector2Flt{ -_puyo->rad() * 2.0f ,0.0f });
	move(_input, INPUT_ID::RIGHT, _puyo, Vector2Flt{ _puyo->rad() * 2.0f ,0.0f });

	_puyo->Update();
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, _size.x, _size.y, _color, true);
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
