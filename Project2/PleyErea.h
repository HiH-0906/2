#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input.h"

class PleyErea
{
public:
	PleyErea();
	~PleyErea();
	void UpDate(void);
	void Init(void);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	std::unique_ptr<Input> _input;
	const Vector2 _ereaSize;

};

