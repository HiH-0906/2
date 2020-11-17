#include "Obj.h"

Obj::Obj(Vector2 pos, Vector2 size,int speed) :pos_(pos), size_(size),speed_(speed)
{
	Update_ = std::bind(&Obj::UpdateDef, this);
	offSetY_ = size_.y / 2;
	animCnt_ = 0;
}

bool Obj::UpdateDef(void)
{
	return false;
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

RevData Obj::PickUpMes(MES_TYPE type)
{
	RevData PickMes = {};
	std::lock_guard<std::mutex> lock(mtx_);
	if (revList_.size() != 0)
	{
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
	}
	
	return PickMes;
}
