#pragma once
#include <vector>
#include <DxLib.h>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct ThreeWay
{
	void operator()(Position2 enemy, Position2 player, std::vector<shardBul>& list)
	{
		auto vec = Position2{ player.x - enemy.x,player.y - enemy.y };
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
		auto angle = atan2f(vec.y, vec.x);
		auto biffAngle = 15 * (DX_PI_F / 180.0f);
		vec = Vector2(cosf((angle + biffAngle)), sinf((angle + biffAngle)));
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
		vec = Vector2(cosf((angle - biffAngle)), sinf((angle - biffAngle)));
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
	}
};