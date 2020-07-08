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
	Vector2Flt _pos;
	float _rad;
	float _speed;
};

