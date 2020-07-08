#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input.h"
#include "Puyo.h"

class PleyErea
{
public:
	PleyErea(Vector2&& size,Vector2&& offset);
	~PleyErea();
	void UpDate(void);
	void Init(Vector2&& size, Vector2&& offset);
	const Vector2 offset(void);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	std::unique_ptr<Input> _input;
	Vector2 _size;
	Vector2 _offset;
	Puyo* _puyo;
};

