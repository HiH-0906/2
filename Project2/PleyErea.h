#pragma once
#include <memory>
#include "Vector2.h"
#include "Input.h"

class PleyErea
{
public:
	PleyErea();
	~PleyErea();
	void UpDate(void);
	void Init(void);
private:
	const Vector2 _ereaSize;				// �����ʻ���
	std::unique_ptr<Input> _input;
};

