#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "Puyo.h"

class PleyErea
{
public:
	PleyErea(Vector2&& size);
	~PleyErea();
	void UpDate(void);
	const Vector2 offset(void);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	PLAYER_ID _playerID;
	const Vector2 _size;
	std::shared_ptr<Input*> _input;
	std::shared_ptr<Puyo> _puyo;
	static int _allStage;
};

