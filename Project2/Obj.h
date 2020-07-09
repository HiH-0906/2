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
	bool _alive;							// ¶‚«‚Ä‚é‚©
	Vector2Flt _pos;						// À•W
	float _rad;								// ”¼Œa
	float _speed;							// —‰º½Ëß°ÄŞ
};

