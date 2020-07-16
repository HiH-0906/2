#pragma once
#include <vector>
#include <DxLib.h>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct Spiral
{
	static int angle;
	void operator()(Position2& enemy, Position2& player, std::vector<shardBul>& list)
	{
		auto vec = Vector2(cosf((angle) * (DX_PI_F / 180.0f)), sinf((angle) * (DX_PI_F / 180.0f)));
		list.emplace_back(new bullet(std::move(enemy), vec * 2));
		list.emplace_back(new bullet(std::move(enemy), vec * -2));
		angle += 10;
	}
};
int Spiral::angle = 0;