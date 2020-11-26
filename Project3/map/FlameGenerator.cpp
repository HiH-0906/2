#include "FlameGenerator.h"
#include "Map.h"

namespace
{
	constexpr __int64 Next_Gene_Time = 1000 / 6;
}

FlameGenerator::FlameGenerator(Vector2 chipPos, int length,Map& mapMng, std::vector<FlameData>& data, Time now)
	: chipPos_(chipPos), mapMng_(mapMng), flameData_(data), length_({ length ,length ,length ,length }), olgLength_({ length ,length ,length ,length })
{
	alive_ = true;
	generatTime_ = 0;
	oldTime_ = now;
}

bool FlameGenerator::Update(const Time& now)
{	
	SetNextFlame(now);
	if (length_.length == 0)
	{
		alive_ = false;
		return false;
	}
	return true;
}

bool FlameGenerator::Alive(void)
{
	return alive_;
}

bool FlameGenerator::SetNextFlame(const Time& now)
{
	if (generatTime_ != 0)
	{
		generatTime_ -= std::chrono::duration_cast<std::chrono::milliseconds>(now - oldTime_).count();
		oldTime_ = now;
		if (generatTime_ < 0)
		{
			generatTime_ = 0;
		}
		return false;
	}
	// ゴミコード
	if (length_.fLength.left != 0)
	{
		auto pos = chipPos_;
		pos.x -= olgLength_.fLength.left - length_.fLength.left;

		auto length = flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)].length_;
		length.fLength.left = length_.fLength.left;
		
			
		if (mapMng_.CheckBreakWall(pos))
		{
			length_.fLength.left = 0;
		}
		else
		{
			length_.fLength.left--;
		}
		FlameData flameData = { Next_Gene_Time * 7,length };
		if (!mapMng_.CheckHitWall(pos))
		{
			flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)] = flameData;
		}
		else
		{
			length_.fLength.left = 0;
		}
	}
	if (length_.fLength.right != 0)
	{
		auto pos = chipPos_;
		pos.x += olgLength_.fLength.right - length_.fLength.right;

		auto length = flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)].length_;
		length.fLength.right = length_.fLength.right;
		
		if (mapMng_.CheckBreakWall(pos))
		{
			length_.fLength.right = 0;
		}
		else
		{
			length_.fLength.right--;
		}
		FlameData flameData = { Next_Gene_Time * 7,length };
		if (!mapMng_.CheckHitWall(pos))
		{
			flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)] = flameData;
		}
		else
		{
			length_.fLength.right = 0;
		}
	}
	if (length_.fLength.up != 0)
	{
		auto pos = chipPos_;
		pos.y -= olgLength_.fLength.up - length_.fLength.up;

		auto length = flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)].length_;
		length.fLength.up = length_.fLength.up;
		if (mapMng_.CheckBreakWall(pos))
		{
			length_.fLength.up = 0;
		}
		else
		{
			length_.fLength.up--;
		}
		FlameData flameData = { Next_Gene_Time * 7,length };
		if (!mapMng_.CheckHitWall(pos))
		{
			flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)] = flameData;
		}
		else
		{
			length_.fLength.up = 0;
		}
	}
	if (length_.fLength.down != 0)
	{
		auto pos = chipPos_;
		pos.y += olgLength_.fLength.down - length_.fLength.down;

		auto length = flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)].length_;
		length.fLength.down = length_.fLength.down;
		if (mapMng_.CheckBreakWall(pos))
		{
			length_.fLength.down = 0;
		}
		else
		{
			length_.fLength.down--;
		}
		FlameData flameData = { Next_Gene_Time * 7,length };
		if (!mapMng_.CheckHitWall(pos))
		{
			flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(mapMng_.GetMapSize().x)] = flameData;
		}
		else
		{
			length_.fLength.down = 0;
		}
	}
	generatTime_ = Next_Gene_Time;
	return true;
}
