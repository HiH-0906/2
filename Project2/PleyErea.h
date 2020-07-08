#pragma once
#include <memory>
#include "common/Vector2.h"
#include "Input.h"
#include "Puyo.h"

class PleyErea
{
public:
	PleyErea(Vector2&& size,Vector2&& offset,PLAYER_ID&& id);
	~PleyErea();
	void UpDate(void);
	void Init(Vector2&& size, Vector2&& offset,PLAYER_ID&& id);
	const Vector2 offset(void);
	const int GetScreenID(void)const;
private:
	void Draw(void);
	int _screenID;
	std::shared_ptr<Input> _input;
	PLAYER_ID _playerID;
	Vector2 _size;
	Vector2 _offset;
	std::shared_ptr<Puyo> _puyo;
};

