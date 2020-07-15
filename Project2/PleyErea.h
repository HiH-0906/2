#pragma once
#include <memory>
#include <vector>
#include <list>
#include "common/Vector2.h"
#include "Input/Input.h"
#include "State/CON_ID.h"
#include "Puyo.h"
#include "State/PUYO_ID.h"

enum class PUYO_STATE
{
	NON,
	STAY,
	DEATH,
};

using PuyoSt = std::pair < PUYO_ID, PUYO_STATE >;

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
	bool CheckMovePuyo(INPUT_ID& id);
	void CheckPuyo(void);
	int _screenID;
	int _playerID;
	const Vector2 _size;
	std::shared_ptr<Input*> _input;
	std::shared_ptr<Puyo> _puyo;
	std::list<std::shared_ptr<Puyo>> _puyoList;
	std::vector<int*> _playErea;
	std::vector<int> _playEreaBase;
	int _color;
	static int _allStage;
};

