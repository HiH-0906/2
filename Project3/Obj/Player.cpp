#include <DxLib.h>
#include "Player.h"
#include "../NetWork/NetWork.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugConOut.h"

int Player::fallCnt_ = 0;

Player::Player(Vector2 pos, Vector2 size, int speed,int id): Obj(pos,size,speed), id_(id), mapMng_(Map::GetInstance())
{
	chPos_ = {};
	lpImageMng.GetID("player", "Image/bomberman.png", { size_.x,size_.y }, { 5,4 });
	dir_ = DIR::DOWN;
	state_ = AnimState::IDEL;
	chipSize_ = Map::GetInstance().GetChipSize();
	auto mode = lpNetWork.GetMode();
	if (mode == NetWorkMode::OFFLINE)
	{
		Update_ = std::bind(&Player::UpdateDef, this);
	}
	else
	{
		int checkID = mode == NetWorkMode::HOST ? 0 : 1;
		if ((id_ % 2) != checkID)
		{
			Update_ = std::bind(&Player::UpdataNet, this);
		}
		else
		{
			Update_ = std::bind(&Player::UpdateDef, this);
		}
	}
	revList_.reserve(100);

	speedVec_.try_emplace(DIR::DOWN, Vector2{ 0,speed });
	speedVec_.try_emplace(DIR::UP, Vector2{ 0,-speed });
	speedVec_.try_emplace(DIR::LEFT, Vector2{ -speed,0 });
	speedVec_.try_emplace(DIR::RIGHT, Vector2{ speed,0 });

	lpNetWork.SetObjRevData(id_, mtx_, revList_);
}

Player::~Player()
{
}

bool Player::UpdateDef(void)
{
	auto dir = dir_;
	while (CheckHitWall(dir))
	{
		++dir;
		if (dir == DIR::MAX)
		{
			dir = begin(DIR());
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


bool Player::UpdataNet(void)
{
	bool test = false;
	 while (isPickMesList(MES_TYPE::POS))
	 {
		 auto mes = PickUpMes(MES_TYPE::POS);
		 if (mes.first.type == MES_TYPE::POS)
		 {
			 auto& data = mes.second;
			 pos_ = Vector2{ static_cast<int>(data[1].idata),static_cast<int>(data[2].idata) };
			 test = true;
		 }
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
	if (((pos_.x % chipSize_.x) == 0) && ((pos_.y % chipSize_.y) == 0))
	{
		chPos_ = mapMng_.ChengeChip(pos_);

		chPos_ += speedVec_[dir] / speed_;
		return mapMng_.CheckHitWall(chPos_);
	}
	return false;
}

void Player::Draw(void)
{
	DrawGraph(pos_.x, pos_.y - offSetY_, lpImageMng.GetID("player")[static_cast<size_t>((animCnt_ / 15) % 2) * 5 + (static_cast<size_t>(state_) * 10)], true);
}
