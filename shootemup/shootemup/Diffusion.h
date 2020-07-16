#pragma once
#include <vector>
#include <DxLib.h>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"


struct Diffusion
{
	void operator()(Position2& enemy, Position2& player, std::vector<shardBul>& list)
	{
		float angle = DX_PI_F / 9.0f;
		for (int i = 0; i <= 18; i++)
		{
			auto vec = Vector2(cosf(angle * static_cast<float>(i)), sinf(angle * static_cast<float>(i)));
			list.emplace_back(new bullet(std::move(enemy), vec * 2));
		}
	}
};