#pragma once
#include <vector>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct Diffusion
{
	void operator()(Position2 enemy, Position2 player, std::vector<shardBul>& list)
	{
		for (int rad = 0; rad <= 360; rad += 20)
		{
			auto vec = Vector2(cos(rad * (3.14 / 180)), sin(rad * (3.14 / 180)));
			list.emplace_back(new bullet(enemy, vec * 2));
		}
	}
};