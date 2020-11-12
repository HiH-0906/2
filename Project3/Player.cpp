#include <DxLib.h>
#include "Player.h"
#include "map/Map.h"
#include "NetWork/NetWork.h"
#include "common/ImageMng.h"


Player::Player(Vector2 pos, Vector2 size, int speed,int id) :pos_(pos), size_(size), speed_(speed), id_(id)
{
	lpImageMng.GetID("player", "Image/bomberman.png", { size_.x,size_.y }, { 5,4 });
	dir_ = DIR::DOWN;
	if ((id_ % 2) == 1)
	{
		update_ = std::bind(&Player::UpdataNet, this);
	}
	else
	{
		update_ = std::bind(&Player::UpdateH, this);
	}
}

Player::~Player()
{
}

void Player::UpdateH(void)
{
	auto CheckDir = [&](DIR dir)
	{
		auto size = Map::GetInstance().GetChipSize();
		if (((pos_.x % size.x) == 0) && ((pos_.y % size.y) == 0))
		{
			Vector2 tmpPos = Map::GetInstance().ChengeChip(pos_);
			
			if (dir == DIR::RIGHT)
			{
				tmpPos.x++;
			}
			if (dir == DIR::DOWN)
			{
				tmpPos.y++;
			}
			if (dir == DIR::LEFT)
			{
				tmpPos.x--;
			}
			if (dir == DIR::UP)
			{
				tmpPos.y--;
			}
			return Map::GetInstance().CheckHitWall(tmpPos);
		}
		return false;
	};
	auto dir = dir_;
	while(CheckDir(dir))
	{
		++dir;
		if (dir == DIR::MAX)
		{
			dir = begin(DIR());
		}
	}
	dir_ = dir;
	Vector2 power = { 0,0 };
	if (dir_==DIR::RIGHT)
	{
		power.x += speed_;
	}
	if (dir_ == DIR::LEFT)
	{
		power.x -= speed_;
	}
	if (dir_ == DIR::UP)
	{
		power.y -= speed_;
	}
	if (dir_ == DIR::DOWN)
	{
		power.y += speed_;
	}
	pos_ += power;
	MesDataList mes;
	sendData pos = { static_cast<unsigned int>(pos_.x) };
	mes.emplace_back(pos);
	pos = { static_cast<unsigned int>(pos_.y) };
	mes.emplace_back(pos);
	lpNetWork.SendMes(MES_TYPE::POS, mes);
};

void Player::Update(void)
{
	update_();
}

void Player::UpdataNet(void)
{
	if (lpNetWork.CheckMes(MES_TYPE::POS))
	{
		auto mes = lpNetWork.PickUpMes();
		if (mes.first.type == MES_TYPE::POS)
		{
			auto data = mes.second;
			pos_ = Vector2{ static_cast<int>(data[0].idata),static_cast<int>(data[1].idata) };
		}
	}
}

void Player::Draw(void)
{
	DrawGraph(pos_.x, pos_.y - (size_.y / 2), lpImageMng.GetID("player")[0], true);
}
