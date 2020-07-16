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
	bool alive_;							// ¶‚«‚Ä‚é‚©
	Vector2 pos_;							// À•W
	int _rad;								// ”¼Œa
	float _speed;							// —‰º½Ëß°ÄŞ
};

