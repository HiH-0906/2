#pragma once
#include<memory>
#include<map>
#include<functional>
#include"Geometry.h"




struct Bullet {
	Position2 pos;			//ç¿ïW
	Vector2 vel;			//ë¨ìx
	float rad;
	bool isActive = false;	//ê∂Ç´ÇƒÇÈÇ©Å`ÅH
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

