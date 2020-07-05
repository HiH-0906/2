#include "BulMng.h"
#include "NormalBullet.h"
#include "ThreeWay.h"
#include "Diffusion.h"
#include "Scattered.h"

std::unique_ptr<BulMng, BulMng::BulMngDeleter> BulMng::_instance(new BulMng());

void BulMng::AddBulQue(BULLET_MOVE type, Position2& enemy, Position2& player)
{
	_queList.push_back(BUL_QUE_T(type, enemy, player));
}

void BulMng::RunBulQue(void)
{
	BULLET_MOVE type;
	Position2 enemy;
	Position2 player;
	for (auto que : _queList)
	{
		std::tie(type, enemy, player) = que;
		_init[type](enemy, player,_bulletList);
	}
	_queList.clear();
}

void BulMng::UpDate(void)
{
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

void BulMng::Draw(void)
{
	for (auto bul : _bulletList)
	{
		(*bul).Draw();
	}
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
	_init.try_emplace(BULLET_MOVE::Normal, NormalBullet());
	_init.try_emplace(BULLET_MOVE::ThreeWay, ThreeWay());
	_init.try_emplace(BULLET_MOVE::Diffusion, Diffusion());
	_init.try_emplace(BULLET_MOVE::Scattered, Scattered());
}

BulMng::~BulMng()
{
	_queList.clear();
	_bulletList.clear();
}
