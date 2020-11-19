#include "Obj.h"
#include"../map/Map.h"

Obj::Obj(Vector2 pos, Vector2 size, int id, int speed, std::shared_ptr<Map> mapMng, BaseScene& scene)
	:pos_(pos), size_(size),id_(id), speed_(speed), mapMng_(mapMng), scene_(scene)
{
	Update_ = std::bind(&Obj::UpdateDef, this);
	offSetY_ = size_.y / 2;
	animCnt_ = 0;
	alive_ = true;
}

bool Obj::UpdateDef(void)
{
	return false;
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
			PickMes = *revList_.begin();
			revList_.erase(revList_.begin() + cnt);
			break;
		}
		cnt++;
	}
	
	return PickMes;
}
