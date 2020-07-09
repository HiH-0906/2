#pragma once
#include "common/Vector2.h"

class Obj
{
public:
	Obj();
	virtual ~Obj();
	const virtual float rad()const;
private:
	void Init(void);
protected: 
	bool _alive;							// 生きてるか
	Vector2Flt _pos;						// 座標
	float _rad;								// 半径
	float _speed;							// 落下ｽﾋﾟｰﾄﾞ
};

