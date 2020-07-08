#pragma once
#include<memory>
#include<map>
#include<functional>
#include"Geometry.h"




struct Bullet {
	Position2 pos;			//���W
	Vector2 vel;			//���x
	float rad;
	bool isActive = false;	//�����Ă邩�`�H
};


class bullet
{
public:
	bullet(Position2 pos,Vector2 vel);
	~bullet();
	void UpDate(void);
	bool isDead(void);
	void SetActiv(bool flag);
	void Draw(bool debug);
	Bullet Getbullet(void)const;
private:
	Bullet _bullet;
	float _angle;
	int image;
};

