#pragma once
#include <vector>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct NormalBullet
{
	void operator()(Position2 enemy, Position2 player,std::vector<shardBul>& list)
	{
		list.emplace_back(new bullet(enemy, (player - enemy).Normalized() * 5));
	}
};

