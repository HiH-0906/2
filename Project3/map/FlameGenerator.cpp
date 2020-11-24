#include "FlameGenerator.h"
#include "Map.h"

FlameGenerator::FlameGenerator(Vector2 chipPos, FlameDIR dir, int length, int time, Map& mapMng, std::vector<int>& data)
	: chipPos_(chipPos), flameDir_(dir), length_(length), time_(time), mapMng_(mapMng), flameData_(data)
{
	alive_ = true;
	cnt_ = 0;
	flameData_[chipPos_.x + static_cast<size_t>(chipPos_.y) * static_cast<size_t>(mapMng_.GetMapSize().x)] = time_;
}

bool FlameGenerator::Update(void)
{
	if (time_ <= cnt_)
	{
		alive_ = false;
		return false;
	}
	
	SetNextFlame();
	cnt_++;
	return true;
}

bool FlameGenerator::Alive(void)
{
	return alive_;
}

bool FlameGenerator::SetNextFlame(void)
{
	if (cnt_ == 10 && length_ != 0)
	{
		if (flameDir_.left != 0)
		{
			auto pos = chipPos_;
			pos.x -= 1;
			if (!mapMng_.CheckHitWall(pos))
			{
				mapMng_.GeneratoFlame(pos, (length_ - 1), FlameDIR{ 1,0,0,0 });
			}
		}
		if (flameDir_.right != 0)
		{
			auto pos = chipPos_;
			pos.x += 1;
			if (!mapMng_.CheckHitWall(pos))
			{
				mapMng_.GeneratoFlame(pos, (length_ - 1), FlameDIR{ 0,1,0,0 });
			}
		}
		if (flameDir_.up != 0)
		{
			auto pos = chipPos_;
			pos.y -= 1;
			if (!mapMng_.CheckHitWall(pos))
			{
				mapMng_.GeneratoFlame(pos, (length_ - 1), FlameDIR{ 0,0,1,0 });
			}
		}
		if (flameDir_.down != 0)
		{
			auto pos = chipPos_;
			pos.y += 1;
			if (!mapMng_.CheckHitWall(pos))
			{
				mapMng_.GeneratoFlame(pos, (length_ - 1), FlameDIR{ 0,0,0,1 });
			}
		}
	}
	return true;
}

