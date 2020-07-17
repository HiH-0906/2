#include "PleyErea.h"
#include "SceneMng.h"
#include "_debug/_DebugConOut.h"
#include "Input/keyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"

int PleyErea::allStage_ = 0;

PleyErea::PleyErea(Vector2&& size, Vector2&& offset, CON_ID id) :size_(size), stgSize_(STAGE_X, STAGE_Y)
{
	offset_ = offset;
	Init(id);
	(*input_)->Setting();
}

PleyErea::~PleyErea()
{
	allStage_--;
}

void PleyErea::UpDate()
{
	(*input_)->Update(playerID_);

	if (downCnt_ >= 30)
	{
		(*input_)->SetDownTrg();
		downCnt_ = 0;
	}

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
		move(input_, id, puyoList_.front());
	}

	puyoList_.front()->Update();
	if (aliveCnt_ >= 30)
	{
		NextPuyo();
	}
	Draw();
	downCnt_++;
}

void PleyErea::Draw(void)
{
	SetDrawScreen(puyoScreenID_);
	ClsDrawScreen();
	DrawBox(0, 0, (STAGE_X) * (PUYO_SIZE), (STAGE_Y)*PUYO_SIZE, color_, true);
	DrawBox(PUYO_SIZE, PUYO_SIZE, (STAGE_X - 1) * PUYO_SIZE, (STAGE_Y - 1) * PUYO_SIZE, 0xffffff, false);
	for (auto list : puyoList_)
	{
		list->Draw();
	}
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	DrawGraph(offset_.x, offset_.y, puyoScreenID_, true);
}

bool PleyErea::CheckMovePuyo()
{
	auto tmpPos = puyoList_.front()->GetGrid(blockSize_);
	DirPermit dirpermit;
	dirpermit.perbit = { 1,1,1,1 };

	if (playErea_[tmpPos.x + 1][tmpPos.y] != PUYO_ID::NON)
	{
		dirpermit.perbit.right = 0;
	}
	if (playErea_[tmpPos.x - 1][tmpPos.y] != PUYO_ID::NON)
	{
		dirpermit.perbit.left = 0;
	}
	if (playErea_[tmpPos.x][tmpPos.y - 1] != PUYO_ID::NON)
	{
		dirpermit.perbit.up = 0;
	}
	if (playErea_[tmpPos.x][tmpPos.y + 1] != PUYO_ID::NON)
	{
		dirpermit.perbit.down = 0;
		aliveCnt_++;
	}
	if (dirpermit.perbit.down == 1)
	{
		aliveCnt_ = 0;
	}
	puyoList_.front()->dirpermit(dirpermit);
	return true;
}

bool PleyErea::Init(CON_ID id)
{
	playerID_ = allStage_;
	allStage_++;
	aliveCnt_ = 0;
	downCnt_ = 0;
	color_ = 0x000066 << (16 * static_cast<int>(playerID_));
	screenID_ = MakeScreen(size_.x, size_.y, true);
	puyoScreenID_ = MakeScreen(stgSize_.x * PUYO_SIZE, stgSize_.y * PUYO_SIZE, true);
	blockSize_ = 32;
	playEreaBase_.resize(stgSize_.x * stgSize_.y);
	puyoList_.emplace_front(std::make_shared<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ }, PUYO_RAD));
	for (int no = 0; no < stgSize_.x; no++)
	{
		playErea_.emplace_back(&playEreaBase_[no * stgSize_.y]);
	}

	for (int x = 0; x < stgSize_.x; x++)
	{
		for (int y = 0; y < stgSize_.y; y++)
		{
			if (x == 0 || x == stgSize_.x - 1 || y == 0 || y == stgSize_.y - 1)
			{
				playErea_[x][y] = PUYO_ID::WALL;
				continue;
			}
			playErea_[x][y] = PUYO_ID::NON;
		}
	}

	switch (id)
	{
	case CON_ID::KEY:
		input_ = std::make_shared<Input*>(new keyState());
		break;
	case CON_ID::PAD:
		input_ = std::make_shared<Input*>(new PadState());
		break;
	case CON_ID::MOUSE:
		input_ = std::make_shared<Input*>(new MouseState());
		break;
	case CON_ID::MAX:
		input_ = std::make_shared<Input*>(new keyState());
		break;
	default:
		TRACE("�R���g���[���[��default");
		break;
	}
	return true;
}

void PleyErea::NextPuyo(void)
{
	auto tmpPos = puyoList_.front()->GetGrid(blockSize_);
	DirPermit dirpermit;
	dirpermit.per = 0;											// �����Ȃ����ā`
	puyoList_.front()->dirpermit(dirpermit);

	playErea_[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y)] = puyoList_.front()->id();
	puyoList_.emplace_front(std::make_shared<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ }, PUYO_RAD));
	aliveCnt_ = 0;
}

const Vector2 PleyErea::offset(void)
{
	return offset_;
}

const int PleyErea::GetScreenID(void)const
{
	return screenID_;
}
