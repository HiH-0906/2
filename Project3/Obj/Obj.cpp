#include "Obj.h"
#include"../map/Map.h"
#include "../_debug/_DebugConOut.h"

Obj::Obj(Vector2 pos, Vector2 size, int id, int speed, std::shared_ptr<Map> mapMng, BaseScene& scene)
	:pos_(pos), size_(size),id_(id), speed_(speed), mapMng_(mapMng), scene_(scene)
{
	Update_ = std::bind(&Obj::UpdateDef, this,std::placeholders::_1);
	offSetY_ = size_.y / 2;
	animCnt_ = 0;
	alive_ = true;
	activ_ = true;
}


bool Obj::UpdateDef(const Time& now)
{
	return false;
}

const Vector2& Obj::GetPos(void) const
{
	return pos_;
}

const Vector2& Obj::GetSize(void) const
{
	return size_;
}

const OBJ_TAG& Obj::GetTag(void)
{
	return tag_;
}

bool Obj::CheckMesList(void)
{
	std::lock_guard<std::mutex> lock(mtx_);
	if (revList_.size() != 0)
	{
		return true;
	}
	return false;
}

int Obj::GetID(void)
{
	return id_;
}

bool Obj::Alive(void)
{
	return alive_;
}

bool Obj::isPickMesList(MES_TYPE type)
{
	std::lock_guard<std::mutex> lock(mtx_);
	if (revList_.size() != 0)
	{
		for (const auto& list : revList_)
		{
			if (list.first.type == type)
			{
				return true;
			}
		}
	}
	return false;
}

RevDataP Obj::PickUpMes(MES_TYPE type)
{
	RevDataP PickMes = {};
	std::lock_guard<std::mutex> lock(mtx_);
	int cnt = 0;
	for (auto& list : revList_)
	{
		if (list.first.type == type)
		{
			PickMes = list;
			revList_.erase(revList_.begin() + cnt);
			break;
		}
		cnt++;
	}
	
	return PickMes;
}

bool Obj::CheckHitWall(DIR dir)
{
	auto tmppos = pos_;

	if (dir == DIR::LEFT)
	{
		tmppos.y += (size_.y / 2);
		tmppos.x--;
	}
	if (dir == DIR::RIGHT)
	{
		tmppos.y += (size_.y / 2);
		tmppos.x += size_.x + 1;
	}
	if (dir == DIR::UP)
	{
		tmppos.x += (size_.x / 2);
		tmppos.y--;
	}
	if (dir == DIR::DOWN)
	{
		tmppos.x += (size_.x / 2);
		tmppos.y += size_.y + 1;
	}

	auto chPos_ = mapMng_->ChengeChip(tmppos);

	return mapMng_->CheckHitWall(chPos_);

}
