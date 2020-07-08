#pragma once
#include <vector>
#include <DxLib.h>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct Scattered
{
	void operator()(Position2 enemy, Position2 player, std::vector<shardBul>& list)
	{
		auto angle = rand() % 60 + 60.0f;
		auto vec = Vector2(cosf(angle * (DX_PI_F / 180.0f)), sinf(angle * (DX_PI_F / 180.0f)));
		list.emplace_back(new bullet(enemy, vec * 5.0f));
	}
};