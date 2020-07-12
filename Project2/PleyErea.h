#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"

class PleyErea
{
public:
	PleyErea(Vector2&& size,CON_ID id);
	~PleyErea();
	void UpDate(void);
	const Vector2 offset(void);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	int _playerID;
	const Vector2 _size;
	std::shared_ptr<Input*> _input;
	std::shared_ptr<Puyo> _puyo;
	int _color;
	static int _allStage;
};

