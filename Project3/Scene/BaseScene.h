#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include "../common/Vector2.h"

class BaseScene;											// ｸﾗｽのﾌﾟﾛﾄﾀｲﾌﾟ宣言

using unipueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unipueBase Update(unipueBase own) = 0;			// どのｼｰﾝにもあるので純粋仮想
	virtual void Draw(void) = 0;
	virtual void Init(void) = 0;
};

