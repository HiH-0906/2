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
	bool alive_;							// �����Ă邩
	Vector2 pos_;							// ���W
	int _rad;								// ���a
	float _speed;							// ������߰��
};

