#pragma once
#include <vector>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"
#include <DxLib.h>

struct Scattered
{
	void operator()(Position2 enemy, Position2 player, std::vector<shardBul>& list)
	{
		for (int i = 0; i < 10; i++)
		{
			auto rad = rand() % 60 + 60;
			auto vec = Vector2(cos(rad * (3.14 / 180)), sin(rad * (3.14 / 180)));
			list.emplace_back(new bullet(enemy, vec * 5));
		}
	}
};