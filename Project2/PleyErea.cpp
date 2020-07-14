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

	_playEreaBase.resize(STAGE_X * STAGE_Y);
	for (int no = 0; no < STAGE_X; no++)
	{
		_playErea.emplace_back(&_playEreaBase[no * STAGE_Y]);
	}
	_puyo = std::make_shared<Puyo>(Vector2Flt{ PUYO_RAD,PUYO_RAD }, PUYO_RAD);
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

	auto move = [&](std::weak_ptr<Input*> input,INPUT_ID IN_id, std::weak_ptr<Puyo> puyo)
	{
		if (!input.expired())
		{
			if ((*input.lock())->GetKeyTrg(IN_id))
			{
				if (CheckMovePuyo(IN_id))
				{
					puyo.lock()->Move(IN_id);
				}
			}
		}
	};
	move(_input, INPUT_ID::LEFT, _puyo);
	move(_input, INPUT_ID::RIGHT, _puyo);
	move(_input, INPUT_ID::UP, _puyo);
	move(_input, INPUT_ID::DOWN, _puyo);

	_puyo->Update();
	CheckPuyo();
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, _size.x, _size.y, _color, true);
	_puyo->Draw();
	for (auto list:_puyoList)
	{
		list->Draw();
	}
}

bool PleyErea::CheckMovePuyo(INPUT_ID& id)
{
	auto tmpPos = _puyo->GetMovePos(id);

	tmpPos = tmpPos - static_cast<float>(PUYO_RAD);

	tmpPos /= PUYO_SIZE;
	if (tmpPos.x >= STAGE_X || tmpPos.y >= STAGE_Y || tmpPos.x < 0 || tmpPos.y < 0)
	{
		return false;
	}
	if (_playErea[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y)].first == PUYO_ID::NON)
	{
		return true;
	}
	return false;
}

void PleyErea::CheckPuyo(void)
{
	auto tmpPos = _puyo->GetMovePos(INPUT_ID::DOWN);
	tmpPos = tmpPos - static_cast<float>(PUYO_RAD);
	tmpPos /= PUYO_SIZE;
	if (tmpPos.y >= STAGE_Y || _playErea[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y)].first != PUYO_ID::NON)
	{
		_puyoList.push_back(std::move(_puyo));
		_playErea[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y - 1)] = { PUYO_ID::RED,PUYO_STATE::STAY };
		_puyo = std::make_shared<Puyo>(Vector2Flt{ PUYO_RAD,PUYO_RAD }, PUYO_RAD);
	}
}

const Vector2 PleyErea::offset(void)
{
	return Vector2{ lpSceneMng.screenSize().x / _allStage * static_cast<int>(_playerID),0 };
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
