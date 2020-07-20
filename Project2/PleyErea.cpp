#include <functional>
#include <algorithm>
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

	bool nextFlag = true;

	std::for_each(puyoList_.rbegin(), puyoList_.rend(), [&](PuyoUnit& puyo) {
		nextFlag &= CheckMovePuyo(puyo);
	});

	if (nextFlag)
	{
		if (mode_ == STAGE_MODE::RENSA)
		{
			puyoList_.emplace(puyoList_.begin(), std::make_unique<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ }, PUYO_RAD));
			CheckMovePuyo(puyoList_[0]);
			mode_ = STAGE_MODE::DROP;
		}
	}

	playUnit_->Update();
	
	bool rensaFlag = true;
	std::for_each(puyoList_.rbegin(), puyoList_.rend(), [&](PuyoUnit& puyo) {
		rensaFlag |= puyo->Update();
	});

	Draw();
}

void PleyErea::Draw(void)
{
	SetDrawScreen(puyoScreenID_);
	ClsDrawScreen();
	DrawBox(0, 0, (STAGE_X) * (PUYO_SIZE), (STAGE_Y)*PUYO_SIZE, color_, true);
	DrawBox(PUYO_SIZE, PUYO_SIZE, (STAGE_X - 1) * PUYO_SIZE, (STAGE_Y - 1) * PUYO_SIZE, 0xffffff, false);
	for (auto&& list : puyoList_)
	{
		list->Draw();
	}
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	DrawGraph(offset_.x, offset_.y, puyoScreenID_, true);
}

bool PleyErea::CheckMovePuyo(PuyoUnit& puyo)
{
	auto tmpPos = puyo->GetGrid(blockSize_);
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
	}
	puyo->dirpermit(dirpermit);
	return true;
}

bool PleyErea::Init(CON_ID id)
{
	mode_ = STAGE_MODE::DROP;
	playerID_ = allStage_;
	allStage_++;
	color_ = 0x000066 << (16 * static_cast<int>(playerID_));
	screenID_ = MakeScreen(size_.x, size_.y, true);
	puyoScreenID_ = MakeScreen(stgSize_.x * PUYO_SIZE, stgSize_.y * PUYO_SIZE, true);
	playUnit_ = std::make_unique<playUnit>(*this);
	blockSize_ = 32;
	playEreaBase_.resize(stgSize_.x * stgSize_.y);
	eraseEreaBase_.resize(stgSize_.x * stgSize_.y);
	for (int no = 0; no < stgSize_.x; no++)
	{
		playErea_.emplace_back(&playEreaBase_[no * stgSize_.y]);
	}
	for (int no = 0; no < stgSize_.x; no++)
	{
		eraseErea_.emplace_back(&eraseEreaBase_[no * stgSize_.y]);
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
		TRACE("ÉRÉìÉgÉçÅ[ÉâÅ[Ç™default");
		break;
	}
	return true;
}

void PleyErea::NextPuyo(void)
{
	auto tmpPos = puyoList_.front()->GetGrid(blockSize_);
	DirPermit dirpermit;
	dirpermit.per = 0;
	puyoList_.front()->dirpermit(dirpermit);
	playErea_[static_cast<int>(tmpPos.x)][static_cast<int>(tmpPos.y)] = puyoList_.front()->id();					// idì¸ÇÍÇƒÅ`
	// è¡Ç∑èàóù
	SetErasePuyo();
	for (auto&& puyo : puyoList_)
	{
		auto vec = puyo->GetGrid(blockSize_);
		if (eraseErea_[vec.x][vec.y] != PUYO_ID::NON)
		{
			puyo->activ(false);
			playErea_[vec.x][vec.y] = PUYO_ID::NON;
		}
	}
	auto itr = std::remove_if(puyoList_.begin(), puyoList_.end(), [](auto&& puyo) {return !(puyo->activ()); });
	puyoList_.erase(itr, puyoList_.end());
	// â∫Ç…óéÇ∆Ç∑èàóù
	

	puyoList_.emplace(
		puyoList_.begin(), std::make_unique<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ }, PUYO_RAD)
		);	// ≤›Ω¿›ΩÇ∑ÇÈÅ`
}

void PleyErea::SetErasePuyo(void)
{
	memset(eraseEreaBase_.data(), 0, eraseEreaBase_.size() * sizeof(PUYO_ID));
	auto vec = puyoList_.front()->GetGrid(blockSize_);
	auto id = puyoList_.front()->id();
	int count = 0;
	std::function<void(PUYO_ID, Vector2)> chPuyo = [&](PUYO_ID id, Vector2 vec) {
		if (eraseErea_[vec.x][vec.y] == PUYO_ID::NON)
		{
			if (playErea_[vec.x][vec.y] == id)
			{
				count++;
				eraseErea_[vec.x][vec.y] = id;
				chPuyo(id, { vec.x + 1,vec.y });
				chPuyo(id, { vec.x - 1,vec.y });
				chPuyo(id, { vec.x,vec.y + 1 });
				chPuyo(id, { vec.x,vec.y - 1 });
			}
		}
	};
	chPuyo(id,vec);
	if (count < 4)
	{
		memset(eraseEreaBase_.data(), 0, eraseEreaBase_.size() * sizeof(PUYO_ID));
	}
}

const Vector2 PleyErea::offset(void)
{
	return offset_;
}

const int PleyErea::GetScreenID(void)const
{
	return screenID_;
}
