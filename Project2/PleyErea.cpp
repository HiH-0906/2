#include "PleyErea.h"
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "Input/keyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"

int PleyErea::_allStage = 0;

PleyErea::PleyErea(Vector2&& size, Vector2&& offset, CON_ID id) :_size(size), _stgSize(STAGE_X, STAGE_Y)
{
	_offset = offset;
	Init(id);
	(*_input)->Setting();
}

PleyErea::~PleyErea()
{
	_allStage--;
}

void PleyErea::UpDate()
{
	(*_input)->Update(_playerID);

	CheckMovePuyo();

	auto move = [](std::weak_ptr<Input*> input,INPUT_ID IN_id, std::weak_ptr<Puyo> puyo)
	{
		if (!input.expired())
		{
			if ((*input.lock())->GetKeyTrg(IN_id))
			{
				puyo.lock()->Move(IN_id);
			}
		}
	};
	for (auto id : INPUT_ID())
	{
		move(_input, id, _puyoList.front());
	}

	_puyoList.front()->Update();
	if (aliveCnt_ >= 30)
	{
		NextPuyo();
	}
	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(_screenID);
	ClsDrawScreen();
	DrawBox(0, 0, (STAGE_X) * (PUYO_SIZE), (STAGE_Y) * PUYO_SIZE, _color, true);
	DrawBox(PUYO_SIZE, PUYO_SIZE, (STAGE_X - 1) * PUYO_SIZE, (STAGE_Y - 1) * PUYO_SIZE, 0xffffff, false);
	for (auto list:_puyoList)
	{
		list->Draw();
	}
}

bool PleyErea::CheckMovePuyo()
{
	auto tmpPos = _puyoList.front()->GetGrid(_blockSize);
	DirPermit dirpermit;
	dirpermit.perbit = { 1,1,1,1 };

	if (_playErea[tmpPos.x + 1][tmpPos.y] != PUYO_ID::NON)
	{
		dirpermit.perbit.right = 0;
	}
	if (_playErea[tmpPos.x - 1][tmpPos.y] != PUYO_ID::NON)
	{
		dirpermit.perbit.left = 0;
	}
	if (_playErea[tmpPos.x][tmpPos.y - 1] != PUYO_ID::NON)
	{
		dirpermit.perbit.up = 0;
	}
	if (_playErea[tmpPos.x][tmpPos.y + 1] != PUYO_ID::NON)
	{
		dirpermit.perbit.down = 0;
		aliveCnt_++;
	}
	if (dirpermit.perbit.down == 1)
	{
		aliveCnt_ = 0;
	}
	_puyoList.front()->dirpermit(dirpermit);
	return true;
}

bool PleyErea::Init(CON_ID id)
{
	_playerID = _allStage;
	_allStage++;
	aliveCnt_ = 0;
	_color = 0x000066 << (16 * static_cast<int>(_playerID));
	_screenID = MakeScreen(_size.x, _size.y, true);
	_blockSize = 32;
	_playEreaBase.resize(_stgSize.x * _stgSize.y);
	_puyoList.emplace_front(std::make_shared<Puyo>(Vector2{ _stgSize.x / 2 * _blockSize,_blockSize }, PUYO_RAD));
	for (int no = 0; no < _stgSize.x; no++)
	{
		_playErea.emplace_back(&_playEreaBase[no * _stgSize.y]);
	}

	for (int x = 0; x < _stgSize.x; x++)
	{
		for (int y = 0; y < _stgSize.y; y++)
		{
			if (x == 0 || x == _stgSize.x - 1 || y == 0 || y == _stgSize.y - 1)
			{
				_playErea[x][y] = PUYO_ID::WALL;
				continue;
			}
			_playErea[x][y] = PUYO_ID::NON;
		}
	}

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
	return true;
}

void PleyErea::NextPuyo(void)
{
	auto tmpPos = _puyoList.front()->GetGrid(_blockSize);
	DirPermit dirpermit;
	dirpermit.per = 0;											// 動けなくして～
	_puyoList.front()->dirpermit(dirpermit);

	_playErea[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y)] = _puyoList.front()->id();
	_puyoList.emplace_front(std::make_shared<Puyo>(Vector2{ _stgSize.x / 2 * _blockSize,_blockSize }, PUYO_RAD));
	aliveCnt_ = 0;
}

const Vector2 PleyErea::offset(void)
{
	return _offset;
}

const int PleyErea::GetScreenID(void)const
{
	return _screenID;
}
