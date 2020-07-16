#pragma once
#include "common/Vector2.h"

class Obj
{
public:
	Obj();
	virtual ~Obj();
	const virtual int rad()const;
private:
	void Init(void);
protected: 
	bool alive_;							// 生きてるか
	Vector2 pos_;							// 座標
	int _rad;								// 半径
	float _speed;							// 落下ｽﾋﾟｰﾄﾞ
};

