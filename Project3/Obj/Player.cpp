#include <DxLib.h>
#include <chrono>
#include "Player.h"
#include "../Scene/BaseScene.h"
#include "../Scene/GameScene.h"
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"
#include "../_debug/_DebugDispOut.h"
#include "../Input/PadState.h"
#include "../Input/keyState.h"
#include"../map/Map.h"

int Player::fallCnt_ = 0;

Player::Player(Vector2 pos, Vector2 size,Vector2 ImageSize, int speed,int id, std::shared_ptr<Map> mapMng, BaseScene& scene): Obj(pos,size,id,speed,mapMng,scene)
{
	chPos_ = {};
	lpImageMng.GetID("player", "Image/bomberman.png", { ImageSize.x,ImageSize.y }, { 5,4 });
	dir_ = DIR::DOWN;
	state_ = AnimState::IDEL;
	chipSize_ = mapMng_->GetChipSize();
	input_ = std::make_unique<keyState>();
	input_->Setting(0, DX_INPUT_PAD1);
	auto mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::OFFLINE)
	{
		if (id_ / UNIT_ID_BASE == 0)
		{
			Update_ = std::bind(&Player::UpdateDef, this);
			FuncInit();
		}
		else
		{
			Update_ = std::bind(&Player::UpdateAuto, this);
		}
	}
	else
	{
		int checkID = mode == NetWorkMode::HOST ? 0 : 1;
		if (id_ / UNIT_ID_BASE == checkID)
		{
			Update_ = std::bind(&Player::UpdateDef, this);
			FuncInit();
		}
		else
		{	
			if (((id_ / UNIT_ID_BASE) % 2) != checkID)
			{
				Update_ = std::bind(&Player::UpdataNet, this);
			}
			else
			{
				Update_ = std::bind(&Player::UpdateAuto, this);
			}
		}
	}
	revList_.reserve(100);

	speedVec_.try_emplace(DIR::DOWN, Vector2{ 0,speed });
	speedVec_.try_emplace(DIR::UP, Vector2{ 0,-speed });
	speedVec_.try_emplace(DIR::LEFT, Vector2{ -speed,0 });
	speedVec_.try_emplace(DIR::RIGHT, Vector2{ speed,0 });

	for (int bomb = 1; bomb <= 4; bomb++)
	{
		StockBomb(id_ + bomb);
	}
	lpNetWork.SetObjRevData(id_, mtx_, revList_);
}

Player::~Player()
{
}

bool Player::UpdateAuto(void)
{
	auto dir = dir_;
	if (((pos_.x % chipSize_.x) == 0) && ((pos_.y % chipSize_.y) == 0))
	{
		while (CheckHitWall(dir))
		{
			++dir;
			if (dir == DIR::MAX)
			{
				dir = begin(DIR());
			}
		}
	}
	dir_ = dir;
	pos_ += speedVec_[dir_];
	data[0] = { static_cast<unsigned int>(id_) };
	data[1] = { static_cast<unsigned int>(pos_.x) };
	data[2] = { static_cast<unsigned int>(pos_.y) };
	data[3] = { static_cast<unsigned int>(dir_) };
	lpNetWork.SendMes(MES_TYPE::POS, MesDataList{ data[0],data[1],data[2],data[3] });
	animCnt_++;
	state_ = AnimState::WALK;
	return true;
}

bool Player::UpdateDef(void)
{
	input_->Update();

	for (auto movePrg = moveFunc_.begin(); movePrg != moveFunc_.end(); movePrg++)
	{
		if ((*movePrg)(true))
		{
			moveFunc_.splice(moveFunc_.begin(), moveFunc_, movePrg);
			moveFunc_.sort([&](moveFunc funcA, moveFunc funcB) {return funcA(false) < funcB(false); });
			break;
		}
	}

	if (input_->GetKeyTrg(INPUT_ID::LROTA))
	{
		auto id = UseBomb();
		if (id >= 0)
		{
			dynamic_cast<GameScene&>(scene_).SetBomb(pos_, id, id_, true, std::chrono::system_clock::now());
		}
	}
	data[0] = { static_cast<unsigned int>(id_) };
	data[1] = { static_cast<unsigned int>(pos_.x) };
	data[2] = { static_cast<unsigned int>(pos_.y) };
	data[3] = { static_cast<unsigned int>(dir_) };
	lpNetWork.SendMes(MES_TYPE::POS, MesDataList{ data[0],data[1],data[2],data[3] });
	animCnt_++;
	state_ = AnimState::WALK;
	return true;
}

bool Player::UpdataNet(void)
{
	bool test = false;
	 while (isPickMesList(MES_TYPE::POS))
	 {
		 auto mes = PickUpMes(MES_TYPE::POS);
			auto& data = mes.second;
			pos_ = Vector2{ static_cast<int>(data[1].uidata),static_cast<int>(data[2].uidata) };
			test = true;
	 }
	 while (isPickMesList(MES_TYPE::SET_BOMB))
	 {
		 auto mes = PickUpMes(MES_TYPE::SET_BOMB);
		 uinonTimeData time = { std::chrono::system_clock::now() };
		 time.idata[0] = mes.second[4].idata;
		 time.idata[1] = mes.second[5].idata;
		 dynamic_cast<GameScene&>(scene_).SetBomb(Vector2{ mes.second[2].idata,mes.second[3].idata }, mes.second[1].idata, mes.second[0].idata, false, time.time);
	 }
	 if (!test)
	 {
		 TRACE("データ未受信ID:%d\n", id_);
		 fallCnt_++;
	 }
	animCnt_++;
	state_ = AnimState::WALK;
	return true;
}

bool Player::CheckHitWall(DIR dir)
{
	auto tmppos = pos_;

	if (dir == DIR::LEFT)
	{
		tmppos.y += (size_.y / 2);
	}
	if (dir == DIR::RIGHT)
	{
		tmppos.y += (size_.y / 2);
		tmppos.x += size_.x;
	}
	if (dir == DIR::UP)
	{
		tmppos.x += (size_.x / 2);
	}
	if (dir == DIR::DOWN)
	{
		tmppos.x += (size_.x / 2);
		tmppos.y += size_.y;
	}

	tmppos += speedVec_[dir] / speed_;

	chPos_ = mapMng_->ChengeChip(tmppos, size_);

	return mapMng_->CheckHitWall(chPos_);
}

void Player::Draw(void)
{
	DrawGraph(pos_.x, pos_.y - offSetY_, lpImageMng.GetID("player")[static_cast<size_t>((animCnt_ / 15) % 2) * 5 + (static_cast<size_t>(state_) * 10)], true);
	_dbgDrawBox(pos_.x, pos_.y, pos_.x + size_.x, pos_.y + size_.y, 0xffffff, false);
}

void Player::StockBomb(int id)
{
	bombList_.emplace_back(id);
}

int Player::UseBomb(void)
{
	int id = -1;
	if (bombList_.size() != 0)
	{
		id = *bombList_.begin();
		bombList_.erase(bombList_.begin());
	}
	return id;
}

void Player::FuncInit(void)
{
	moveFunc_.emplace_back([&](bool tmp)
	{
		if (input_->GetKeySty(INPUT_ID::DOWN))
		{
			if (tmp)
			{
				if (!CheckHitWall(DIR::DOWN))
				{
					pos_ += speedVec_[DIR::DOWN];
					if (pos_.x % (size_.x) != 0)
					{
						pos_.x = ((pos_.x + (size_.x / 2)) / chipSize_.x) * chipSize_.x;
					}
					dir_ = DIR::DOWN;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp)
	{
		if (input_->GetKeySty(INPUT_ID::LEFT))
		{
			if (tmp)
			{
				if (!CheckHitWall(DIR::LEFT))
				{
					pos_ += speedVec_[DIR::LEFT];
					if (pos_.y % (32) != 0)
					{
						pos_.y = ((pos_.y + (size_.y / 2)) / chipSize_.y) * chipSize_.y;
					}
					dir_ = DIR::LEFT;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp)
	{
		if (input_->GetKeySty(INPUT_ID::RIGHT))
		{
			if (tmp)
			{
				if (!CheckHitWall(DIR::RIGHT))
				{
					pos_ += speedVec_[DIR::RIGHT];
					if (pos_.y % (32) != 0)
					{
						pos_.y = ((pos_.y + (size_.y / 2)) / chipSize_.y) * chipSize_.y;
					}
					dir_ = DIR::RIGHT;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
	moveFunc_.emplace_back([&](bool tmp)
	{
		if (input_->GetKeySty(INPUT_ID::UP))
		{
			if (tmp)
			{
				if (!CheckHitWall(DIR::UP))
				{
					pos_ += speedVec_[DIR::UP];
					if (pos_.x % (size_.x) != 0)
					{
						pos_.x = ((pos_.x + (size_.x / 2)) / chipSize_.x) * chipSize_.x;
					}
					dir_ = DIR::UP;
					return true;
				}
				return false;
			}
			return true;
		}
		return false;
	});
}
