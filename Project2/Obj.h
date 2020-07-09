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
	bool _alive;							// �����Ă邩
	Vector2Flt _pos;						// ���W
	float _rad;								// ���a
	float _speed;							// ������߰��
};

