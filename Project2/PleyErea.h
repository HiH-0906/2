#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input.h"

class PleyErea
{
public:
	PleyErea(Vector2 size,Vector2 offset);
	~PleyErea();
	void UpDate(void);
	void Init(Vector2 size, Vector2 offset);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	std::unique_ptr<Input> _input;
	Vector2 _ereaSize;
	Vector2 _offset;

};

