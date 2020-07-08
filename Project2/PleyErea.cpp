#include "PleyErea.h"

PleyErea::PleyErea(Vector2&& size, Vector2&& offset,PLAYER_ID&& id)
{
	Init(std::move(size), std::move(offset),std::move(id));
	_puyo = std::make_shared<Puyo>(std::move(Vector2Flt{ 24.0f,24.0f }), std::move(24.0f));
}

PleyErea::~PleyErea()
{
}

void PleyErea::UpDate()
{
	_input->Update(_playerID);

	auto move = [](std::weak_ptr<Input> input,PLAYER_ID P_id,INPUT_ID IN_id, std::weak_ptr<Puyo> puyo,const Vector2Flt vec)
	{
		if (!input.expired())
		{
			if (input.lock()->GetKeyTrg(P_id, IN_id))
			{
				puyo.lock()->Move(vec);
			}
		}
	};
	move(_input, _playerID, INPUT_ID::LEFT, _puyo, Vector2Flt{ -_puyo->rad() * 2 ,0.0f });
	move(_input, _playerID, INPUT_ID::RIGHT, _puyo, Vector2Flt{ _puyo->rad() * 2 ,0.0f });

	_puyo->Update();
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, _size.x, _size.y, 0xff8888, true);
	_puyo->Draw();
}

void PleyErea::Init(Vector2&& size, Vector2&& offset,PLAYER_ID&& id)
{
	_size = std::move(size);
	_offset = std::move(offset);
	_playerID = std::move(id);
	_input = std::make_shared<Input>();
	_screenID = MakeScreen(_size.x, _size.y, true);
}

const Vector2 PleyErea::offset(void)
{
	return _offset;
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
