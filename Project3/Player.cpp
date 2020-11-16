#include <DxLib.h>
#include "Player.h"
#include "map/Map.h"
#include "NetWork/NetWork.h"
#include "common/ImageMng.h"
#include "_debug/_DebugConOut.h"

Player::Player(Vector2 pos, Vector2 size, int speed,int id): Obj(pos,size,speed), id_(id)
{
	lpImageMng.GetID("player", "Image/bomberman.png", { size_.x,size_.y }, { 5,4 });
	dir_ = DIR::DOWN;
	state_ = AnimState::IDEL;
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
	revList_.reserve(10);

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
	auto CheckDir = [&](DIR dir)
	{
		auto size = Map::GetInstance().GetChipSize();
		if (((pos_.x % size.x) == 0) && ((pos_.y % size.y) == 0))
		{
			Vector2 tmpPos = Map::GetInstance().ChengeChip(pos_);

			tmpPos += speedVec_[dir] / speed_;
			return Map::GetInstance().CheckHitWall(tmpPos);
		}
		return false;
	};
	auto dir = dir_;
	while (CheckDir(dir))
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
	while (CheckMesList(MES_TYPE::POS))
	{
		auto mes = PickUpMes(MES_TYPE::POS);
		if (mes.first.type == MES_TYPE::POS)
		{
			auto& data = mes.second;
			pos_ = Vector2{ static_cast<int>(data[1].idata),static_cast<int>(data[2].idata) };
		}
	}
	animCnt_++;
	state_ = AnimState::WALK;
	return true;
}

void Player::Draw(void)
{
	DrawGraph(pos_.x, pos_.y - offSetY_, lpImageMng.GetID("player")[static_cast<size_t>((animCnt_ / 15) % 2) * 5 + (static_cast<size_t>(state_) * 10)], true);
	//DrawFormatString(pos_.x, pos_.y, 0xffffff, "%d", id_);
}
