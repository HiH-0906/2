#pragma once
#include <vector>
#include"Geometry.h"
#include "bullet.h"
#include "BulMng.h"

struct ThreeWay
{
	void operator()(Position2 enemy, Position2 player, std::vector<shardBul>& list)
	{
		auto vec = Position2{ player.x - enemy.x,player.y - enemy.y };
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
		auto rad = atan2f(vec.y, vec.x);
		auto radEX = 15 * (3.14 / 180);
		vec = Vector2(cos((rad + radEX)), sin((rad + radEX)));
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
		vec = Vector2(cos((rad - radEX)), sin((rad - radEX)));
		list.emplace_back(new bullet(enemy, vec.Normalized() * 5));
	}
};