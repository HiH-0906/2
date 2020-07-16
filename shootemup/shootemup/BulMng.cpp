#include <algorithm>
#include "BulMng.h"
#include "NormalBullet.h"
#include "ThreeWay.h"
#include "Diffusion.h"
#include "Scattered.h"
#include "Spiral.h"
#include "randBullet.h"

std::unique_ptr<BulMng, BulMng::BulMngDeleter> BulMng::_instance(new BulMng());

void BulMng::AddBulQue(BULLET_MOVE type, Position2& enemy, Position2& player)
{
	_queList.push_back(BUL_QUE_T(type, enemy, player));
}

void BulMng::RunBulQue(void)
{
	static BULLET_MOVE type;
	if (_queList.size() == 0)
	{
		return;
	}
	type = std::get<0>(*_queList.begin());
	static Position2& enemy = std::get<1>(*_queList.begin());
	static Position2& player = std::get<2>(*_queList.begin());
	_init[type].first(enemy, player,_bulletList);
	_queList.clear();
}

void BulMng::UpDate(void)
{
	RunBulQue();
	for (auto bul:_bulletList)
	{
		(*bul).UpDate();
	}
	_bulletList.erase(std::remove_if(						// Ç¢ÇÁÇ»Ç¢íeèàóù
		_bulletList.begin(),
		_bulletList.end(),
		[](shardBul& bul) {return (*bul).isDead(); }
	),
		_bulletList.end());
}

void BulMng::Draw(bool debug,int& image)
{
	for (auto bul : _bulletList)
	{
		(*bul).Draw(debug, image);
	}
}

int BulMng::GetBulCnt(BULLET_MOVE type)
{
	return _init[type].second;
}

bool BulMng::HitCheck(Position2& player, float rad)
{
	bool flag = false;
	for (auto bul : _bulletList)
	{
		auto tmpbul = (*bul).Getbullet();
		auto tmp = tmpbul.pos - player;
		if (hypotf(tmp.x, tmp.y) <= tmpbul.rad + rad)
		{
			(*bul).SetActiv(false);
			flag = true;
		}
	}
	return flag;
}

BulMng::BulMng()
{
	_init.try_emplace(BULLET_MOVE::Normal, NormalBullet(),30);
	_init.try_emplace(BULLET_MOVE::ThreeWay, ThreeWay(),60);
	_init.try_emplace(BULLET_MOVE::Diffusion, Diffusion(),45);
	_init.try_emplace(BULLET_MOVE::Scattered, Scattered(),5);
	_init.try_emplace(BULLET_MOVE::Spiral, Spiral(), 5);
	_init.try_emplace(BULLET_MOVE::randBullet, randBullet(), 1);
}

BulMng::~BulMng()
{
	_queList.clear();
	_bulletList.clear();
}
