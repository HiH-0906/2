#include <functional>
#include <algorithm>
#include "_debug/_DebugConOut.h"
#include "PleyErea.h"
#include "NextPuyoCtl.h"
#include "Scene/SceneMng.h"
#include "ImageMng.h"
#include "EffectMng.h"
#include "Puyo/OzyamaPuyo.h"
#include "Input/keyState.h"
#include "Input/PadState.h"
#include "Input/MouseState.h"
#include "PuyoCtl/DropMode.h"
#include "PuyoCtl/EraseMode.h"
#include "PuyoCtl/FallMode.h"
#include "PuyoCtl/MunyonMode.h"
#include "PuyoCtl/PuyonMode.h"
#include "PuyoCtl/OzyamaMode.h"
#include "PuyoCtl/GameOverMode.h"
#include "PuyoCtl/WinMode.h"


int PleyErea::allStage_ = 0;

PleyErea::PleyErea(Vector2&& size, Vector2&& offset, Vector2&& pos, CON_ID id) :size_(size), stgSize_(STAGE_X, STAGE_Y)
{
	offset_ = offset;
	pos_ = pos;
	Init(id);
	for (auto id:CON_ID())
	{
		(*input_[id])->Setting(playerID_, padNum_);
	}
}

PleyErea::~PleyErea()
{
	// �S�̽ð�ސ���������Ă���
	allStage_--;
}

int PleyErea::UpDate()
{
	int reNum = stageFunc_[mode_](*this);
	(*input_[inputID_])->Update();
	if (reNum == -1)
	{
		// �����Źްѵ��ް�Ɉڍs
		TRACE("GAME OVER\n");
		mode_ = STAGE_MODE::GAMEOVER;
	}
	Draw();
	return reNum;
}

void PleyErea::InstancePuyo(void)
{
	auto pairPuyo = nextPuyo_->PickUp();
	pairPuyo.first->pos({ stgSize_.x / 2 * blockSize_, -blockSize_ });
	pairPuyo.second->pos({ stgSize_.x / 2 * blockSize_,0 });

	// �ݽ�ݽ���Ă��邾���`
	puyoList_.emplace(puyoList_.begin(), pairPuyo.first);
	puyoList_.emplace(puyoList_.begin() + 1, pairPuyo.second);
}

void PleyErea::Draw(void)
{
	// �Ղ摀��ꏊ�`��
	SetDrawScreen(puyoScreenID_);
	ClsDrawScreen();
	Vector2 size = stgSize_;
	size *= 32;
	auto bgPos = pos_ + offset_ + (size / 2);
	lpSceneMng.AddDrawList({ bgPos,IMAGE_ID("PUYOBG")[0],rad_,0.0,-1,SCREEN_ID::PLAY ,DATA_TYPE::IMG ,true });
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
	DrawGraph(0, 0, IMAGE_ID("FREAM")[0], true);
	if (puyoList_[0]->playPuyo() || puyoList_[1]->playPuyo())
	{
		DrawGost();
	}
	DrawOzyama();
	// playErea�S�̂̕`��
	SetDrawScreen(screenID_);
	ClsDrawScreen();
	
	nextPuyo_->Draw();
	DrawGraph(offset_.x, offset_.y, puyoScreenID_, true);
	DrawGraph(offset_.x, offset_.y, noticeOzyamaScrID_, true);

	auto tmpPos = pos_ + (size_ / 2);
	lpSceneMng.AddDrawList({ tmpPos,screenID_,rad_,0.0,1,SCREEN_ID::PLAY ,DATA_TYPE::IMG ,true});
}

void PleyErea::DrawOzyama(void)
{
	auto idBefor = GetDrawScreen();
	SetDrawScreen(noticeOzyamaScrID_);
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
	SetDrawScreen(idBefor);
}

void PleyErea::DrawGost(void)
{
	int idBuff = GetDrawScreen();
	SetDrawScreen(gostScreen_);
	ClsDrawScreen();

	auto gostListBase_ = playEreaBase_;
	std::vector<sharPuyo*> gostList;
	for (int no = 0; no < stgSize_.x; no++)
	{
		gostList.emplace_back(&gostListBase_[no * stgSize_.y]);
	}
	for (int i = 1; i >= 0; i--)
	{
		auto gostPos = (*(puyoList_.begin() + i))->GetGrid(blockSize_);
		while (!CheckGost(gostPos, gostList))
		{
			gostPos.y++;
		}
		gostPos *= blockSize_;
		gostPos += 16;
		DrawCircle(gostPos.x, gostPos.y, 4, (*(puyoList_.begin() + i))->GetColor(), true);
	}
	Vector2 size = stgSize_;
	size *= 32;
	auto tmpPos = pos_ + offset_ + (size / 2);
	lpSceneMng.AddDrawList({ tmpPos,gostScreen_,rad_,0.0,0,SCREEN_ID::PLAY ,DATA_TYPE::IMG ,true });
	SetDrawScreen(idBuff);
}

bool PleyErea::CheckMovePuyo(sharPuyo& puyo)
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

bool PleyErea::CheckGost(Vector2& pos, std::vector<sharPuyo*> gost)
{
	// �����Ă�������Bit���
	bool reFlag = false;
	if (pos.y >= 0)
	{
		if (gost[static_cast<size_t>(pos.x)][static_cast<size_t>(pos.y + 1)])
		{
			gost[pos.x][pos.y] = std::make_shared<Puyo>(Vector2{ 0,0 }, PUYO_ID::RED);					// id����ā`
			reFlag = true;
		}
	}
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
	stageFunc_.try_emplace(STAGE_MODE::GAMEOVER, GameOverMode());
	stageFunc_.try_emplace(STAGE_MODE::WIN, WinMode());
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
	gostScreen_ = MakeScreen(stgSize_.x * PUYO_SIZE, stgSize_.y * PUYO_SIZE, true);
	puyoScreenID_ = MakeScreen(stgSize_.x * PUYO_SIZE, stgSize_.y * PUYO_SIZE, true);
	noticeOzyamaScrID_ = MakeScreen(stgSize_.x * PUYO_SIZE, PUYO_SIZE, true);
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
	input_.try_emplace(CON_ID::KEY, std::make_shared<Input*>(new keyState()));
	input_.try_emplace(CON_ID::PAD, std::make_shared<Input*>(new PadState()));
	input_.try_emplace(CON_ID::MOUSE, std::make_shared<Input*>(new MouseState()));

	inputID_ = id;

	Vector2 pos = { blockSize_ * (stgSize_.x + 2),blockSize_ * 3 };
	pos += offset_+ pos_;
	nextPuyo_ = std::make_unique<NextPuyoCtl>(pos, 3, 2);
	// �Ղ�̲ݽ�ݽ
	InstancePuyo();
	
	lpImageMng.GetID("WIN", "image/Win.png");
	lpImageMng.GetID("LOSE", "image/Lose.png");
	lpImageMng.GetID("FREAM", "image/�킭.png");
	lpImageMng.GetID("PUYOBG", "image/puyobg.png");

	padNum_ = 0;
	rad_ = 1.0;

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
				auto efPos = offset_ + puyo->pos() + (blockSize_ / 2);
				efPos += pos_;
				lpEffectMng.SetEffect("������", efPos,SCREEN_ID::PLAY);
				playErea_[vec.x][vec.y].reset();
			}
		}
		eraseCnt_ += count;
		return true;
	}
}

const Vector2& PleyErea::offset(void)
{
	return offset_;
}

const int& PleyErea::GetScreenID(void)const
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

const int& PleyErea::playerID(void) const
{
	return playerID_;
}

void PleyErea::SetWinner(bool winner)
{
	if (winner && mode_ != STAGE_MODE::GAMEOVER)
	{
		mode_ = STAGE_MODE::WIN;
	}
}

const Vector2& PleyErea::pos(void) const
{
	return pos_;
}

const CON_ID& PleyErea::inputID(void) const
{
	return inputID_;
}

const int& PleyErea::padNum(void) const
{
	return padNum_;
}

bool PleyErea::PlesePose(void)
{
	return (*input_[inputID_])->GetKeyTrg(INPUT_ID::POSE);
}

std::map<CON_ID, std::shared_ptr<Input*>> PleyErea::GetInput(void)
{
	return input_;
}

void PleyErea::padNum(int& num)
{
	padNum_ = num;
	(*input_[inputID_])->SetPadNum(std::move(num));
}

void PleyErea::inputID(CON_ID&& id)
{
	inputID_ = id;
}

