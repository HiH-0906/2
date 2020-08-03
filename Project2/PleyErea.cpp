#include <functional>
#include <algorithm>
#include "_debug/_DebugConOut.h"
#include "PleyErea.h"
#include "SceneMng.h"
#include "OzyamaPuyo.h"
#include "Input/keyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"
#include "PuyoCtl/DropMode.h"
#include "PuyoCtl/EraseMode.h"
#include "PuyoCtl/FallMode.h"
#include "PuyoCtl/MunyonMode.h"
#include "PuyoCtl/PuyonMode.h"
#include "PuyoCtl/OzyamaMode.h"


int PleyErea::allStage_ = 0;

PleyErea::PleyErea(Vector2&& size, Vector2&& offset, CON_ID id) :size_(size), stgSize_(STAGE_X, STAGE_Y)
{
	offset_ = offset;
	Init(id);
	(*input_)->Setting();
}

PleyErea::~PleyErea()
{
	// �S�̽ð�ސ���������Ă���
	allStage_--;
}

void PleyErea::UpDate()
{
	(*input_)->Update(playerID_);
	if (!stageFunc_[mode_](*this))
	{
		// �����Źްѵ��ް�Ɉڍs���܂��傤�˂��`
		TRACE("GAME OVER\n");
	}
	Draw();
}

void PleyErea::InstancePuyo(void)
{
	// �ݽ�ݽ���Ă��邾���`
	puyoList_.emplace(
		puyoList_.begin(), std::make_unique<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ }, nextPuyo_[0]->id())
		);
	puyoList_.emplace(
		puyoList_.begin() + 1, std::make_unique<Puyo>(Vector2{ stgSize_.x / 2 * blockSize_,blockSize_ * 2 }, nextPuyo_[1]->id())
		);
	nextPuyo_[0] = std::make_shared<Puyo>(Vector2{ blockSize_ * (stgSize_.x + 2), blockSize_ * 2 }, static_cast<PUYO_ID>((rand() % 5) + 1));
	nextPuyo_[1] = std::make_shared<Puyo>(Vector2{ blockSize_ * (stgSize_.x + 2), blockSize_ * 3 }, static_cast<PUYO_ID>((rand() % 5) + 1));

}

void PleyErea::Draw(void)
{
	// �Ղ摀��ꏊ�`��
	SetDrawScreen(puyoScreenID_);
	ClsDrawScreen();
	DrawBox(0, 0, (STAGE_X) * (PUYO_SIZE), (STAGE_Y)*PUYO_SIZE, color_, true);
	DrawBox(PUYO_SIZE, PUYO_SIZE, (STAGE_X - 1) * PUYO_SIZE, (STAGE_Y - 1) * PUYO_SIZE, 0xffffff, false);
	// �Ղ�[�񎞂ǂꂾ�����ނ�
	for (auto&& list : puyoList_)
	{
		auto vec = list->GetGrid(PUYO_SIZE);
		int cnt = 0;
		for (int y = vec.y + 1; y < STAGE_Y; y++)
		{
			if (!playErea_[vec.x][y])
			{
				break;
			}
			if (!playErea_[vec.x][y]->CheckPuyonCnt())
			{
				break;
			}
			if (++cnt >= 3)
			{
				break;
			}
		}

		list->Draw(cnt);
	}
	DrawOzyama();
	// playErea�S�̂̕`��
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	for (auto puyo: nextPuyo_)
	{
		DrawOval(static_cast<int>(offset_.x + puyo->pos().x + PUYO_RAD), static_cast<int>(offset_.y + puyo->pos().y + PUYO_RAD), static_cast<int>(PUYO_RAD), static_cast<int>(PUYO_RAD), puyo->GetColor(), true);
	}

	DrawGraph(offset_.x, offset_.y, puyoScreenID_, true);
	DrawGraph(offset_.x, offset_.y, NoticeOzyamaScrID, true);
	for (auto tmp : playEreaBase_)
	{
		if (tmp)
		{
			DrawFormatString(offset_.x+tmp->pos().x, offset_.y+ tmp->pos().y, 0xffffff, "%d", tmp->id());
		}
	}
}

void PleyErea::DrawOzyama(void)
{
	SetDrawScreen(NoticeOzyamaScrID);
	ClsDrawScreen();
	if (ozyamaCnt_ <= 0)
	{
		return;
	}
	int size = blockSize_ / 2;
	for (int i = 0; i < ozyamaCnt_; i++)
	{
		DrawCircle((size * (i % 12) + 8) + blockSize_, (24 - (size * (i / 12))), 8, 0x888888, true);
	}
}

bool PleyErea::CheckMovePuyo(PuyoUnit& puyo)
{
	// �����Ă�������Bit���
	auto tmpPos = puyo->GetGrid(blockSize_);
	bool reFlag = false;
	DirPermit dirpermit;
	// ��͂����Ȃ�
	dirpermit.perbit = { 0,1,1,1 };
	// �͂ݏo�Ă���ꍇ�̵̾�Čv�Z
	int offsetY = (puyo->pos().y % blockSize_ != 0);
	if (tmpPos.y >= 0)
	{
		if (playErea_[static_cast<size_t>(tmpPos.x + 1)][static_cast<size_t>(tmpPos.y + offsetY)])
		{
			dirpermit.perbit.right = 0;
		}
		if (playErea_[static_cast<size_t>(tmpPos.x - 1)][static_cast<size_t>(tmpPos.y + offsetY)])
		{
			dirpermit.perbit.left = 0;
		}
		if (playErea_[static_cast<size_t>(tmpPos.x)][static_cast<size_t>(tmpPos.y - 1)])
		{
			dirpermit.perbit.up = 0;
		}
		if (playErea_[static_cast<size_t>(tmpPos.x)][static_cast<size_t>(tmpPos.y + 1)])
		{
			// ���Ɉړ��ł��Ȃ��Ƃ������Ƃ͒��n����������[���Ƃ��!!
			dirpermit.perbit.down = 0;
			auto vec = puyo->GetGrid(blockSize_);
			playErea_[vec.x][vec.y] = puyo;					// id����ā`
			puyo->playPuyo(false);
			reFlag = true;
		}
	}
	// �ݒ�
	puyo->dirpermit(dirpermit);
	return reFlag;
}

bool PleyErea::Init(CON_ID id)
{
	// �֐��o�^
	stageFunc_.try_emplace(STAGE_MODE::DROP,DropMode());
	stageFunc_.try_emplace(STAGE_MODE::ERASE, EraseMode());
	stageFunc_.try_emplace(STAGE_MODE::MUNYON, MunyonMode());
	stageFunc_.try_emplace(STAGE_MODE::PUYON, PuyonMode());
	stageFunc_.try_emplace(STAGE_MODE::FALL, FallMode());
	stageFunc_.try_emplace(STAGE_MODE::OZYAMA, OzyamaMode());
	mode_ = STAGE_MODE::DROP;
	playerID_ = allStage_;
	allStage_++;
	// �����A����
	rensaNum_ = 0;
	ozyamaCnt_ = 0;
	eraseCnt_ = 0;
	rensaMax_ = 2;
	ozyamaFallMax_ = 24;
	// �ð�ޔw�i�F�ݒ�
	color_ = 0x000066 << (16 * static_cast<int>(playerID_));
	// �`��潸ذݍ쐬
	screenID_ = MakeScreen(size_.x, size_.y, true);
	puyoScreenID_ = MakeScreen(stgSize_.x * PUYO_SIZE, stgSize_.y * PUYO_SIZE, true);
	NoticeOzyamaScrID = MakeScreen(stgSize_.x * PUYO_SIZE, PUYO_SIZE, true);
	// ����֌W
	playUnit_ = std::make_unique<playUnit>(*this);
	// Ͻ�ڻ���
	blockSize_ = 32;
	// �ް��ް��쐬
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
		playErea_[x][stgSize_.y - 1] = std::make_shared<Puyo>(Vector2{ x * blockSize_ ,(stgSize_.y - 1) * blockSize_ }, PUYO_ID::WALL);
	}
	for (int y = 0; y < stgSize_.y; y++)
	{
		playErea_[0][y] = std::make_shared<Puyo>(Vector2{ 0, blockSize_ * y }, PUYO_ID::WALL);
		playErea_[stgSize_.x - 1][y] = std::make_shared<Puyo>(Vector2{ (stgSize_.x - 1) * blockSize_,(y) * blockSize_ }, PUYO_ID::WALL);
	}
	// input�쐬
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
	nextPuyo_[0] = std::make_shared<Puyo>(Vector2{ blockSize_ * (stgSize_.x + 2),blockSize_ * 2 }, static_cast<PUYO_ID>((rand() % 5) + 1));
	nextPuyo_[1] = std::make_shared<Puyo>(Vector2{ blockSize_ * (stgSize_.x + 2), blockSize_ * 3 }, static_cast<PUYO_ID>((rand() % 5) + 1));
	// �Ղ�̲ݽ�ݽ
	InstancePuyo();
	
	return true;
}

Vector2 PleyErea::ConvertGrid(Vector2&& pos)
{
	// �󂯎����pos��Ͻ�ڂ�
	return Vector2{ pos.x / blockSize_,pos.y / blockSize_ };
}

void PleyErea::DeletePuyo(void)
{
	// �Ղ�̂��Ȃ��Ȃ������̍폜
	auto itr = std::remove_if(puyoList_.begin(), puyoList_.end(), [](auto&& puyo) {return !(puyo->activ()); });
	if (itr != puyoList_.end())
	{
		puyoList_.erase(itr, puyoList_.end());
	}
}

bool PleyErea::SetErasePuyo(Vector2 vec, PUYO_ID id)
{
	// ����Ȃ�Ղ�
	memset(eraseEreaBase_.data(), 0, eraseEreaBase_.size() * sizeof(PUYO_ID));
	// �폜���ް�������
	for (auto&& puyo : puyoList_)
	{
		auto vec = puyo->GetGrid(blockSize_);
		eraseErea_[vec.x][vec.y].reset();
	}
	int count = 0;
	std::function<void(PUYO_ID, Vector2)> chPuyo = [&](PUYO_ID id, Vector2 vec) {
		if (!eraseErea_[vec.x][vec.y])
		{
			if (playErea_[vec.x][vec.y])
			{
				if (playErea_[vec.x][vec.y]->id() == PUYO_ID::OZAYMA)
				{
					eraseErea_[vec.x][vec.y] = playErea_[vec.x][vec.y];
					return;
				}
				if (playErea_[vec.x][vec.y]->id() == id)
				{
					count++;
					eraseErea_[vec.x][vec.y] = playErea_[vec.x][vec.y];
					chPuyo(id, { vec.x + 1,vec.y });
					chPuyo(id, { vec.x - 1,vec.y });
					chPuyo(id, { vec.x,vec.y + 1 });
					chPuyo(id, { vec.x,vec.y - 1 });
				}
			}
		}
	};
	chPuyo(id,vec);
	if (count < 4)
	{
		for (auto&& puyo : puyoList_)
		{
			auto vec = puyo->GetGrid(blockSize_);
			eraseErea_[vec.x][vec.y].reset();
		}
		return false;
	}
	else
	{
		for (auto&& puyo : puyoList_)
		{
			auto vec = puyo->GetGrid(blockSize_);
			if (eraseErea_[vec.x][vec.y])
			{
				puyo->activ(false);
				playErea_[vec.x][vec.y].reset();
			}
		}
		eraseCnt_ += count;
		return true;
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

void PleyErea::ozyamaCnt(int cnt)
{
	ozyamaCnt_ += cnt;
}

void PleyErea::FallOzyama()
{
	if (ozyamaCnt_ <= 0)
	{
		return;
	}
	// ������芴
	int offset = rand() % 6;
	int count = 0;
	for (int cnt = 0; cnt < ozyamaCnt_; cnt++)
	{
		Vector2 vec = { ((offset + cnt) % (stgSize_.x - 2)) ,((offset + cnt) / (stgSize_.x - 2)) };
		puyoList_.emplace(
			puyoList_.begin(), std::make_shared<OzyamaPuyo>(Vector2{ blockSize_+(blockSize_ * vec.x),-(blockSize_ * vec.y) }, PUYO_ID::OZAYMA)
			);
		CheckMovePuyo(puyoList_[0]);
		puyoList_[0]->ChengeSpeed(16, 1);
		if (cnt >= ozyamaFallMax_)
		{
			break;
		}
		count++;
	}
	ozyamaCnt_ -= count;
}

const int PleyErea::playerID(void) const
{
	return playerID_;
}

