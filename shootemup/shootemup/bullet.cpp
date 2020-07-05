#include<cmath>
#include <DxLib.h>
#include "bullet.h"
#include "NormalBullet.h"

bullet::bullet(Position2 pos, Vector2 vel)
{
	_bullet.pos = pos;
	_bullet.isActive = true;
	_bullet.vel = vel;
	_bullet.rad = 5.0f;
	_angle = atan2(_bullet.vel.y, _bullet.vel.x);
	image = LoadGraph("img/bullet.png");
}

bullet::~bullet()
{
}

void bullet::UpDate(void)
{
	_bullet.pos += _bullet.vel;

	if (_bullet.pos.x + 16 < 0 || _bullet.pos.x - 16 > 640 ||
		_bullet.pos.y + 24 < 0 || _bullet.pos.y - 24 > 480) {
		_bullet.isActive = false;
	}
}

bool bullet::isDead(void)
{
	if (_bullet.isActive == false)
	{
		return true;
	}
	return false;
}

void bullet::SetActiv(bool flag)
{
	_bullet.isActive = flag;
}

void bullet::Draw(void)
{
	DrawRotaGraph(_bullet.pos.x, _bullet.pos.y, 1.0f, _angle, image, true);
}

Bullet bullet::Getbullet(void) const
{
	return _bullet;
}
