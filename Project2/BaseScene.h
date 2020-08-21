#pragma once
#include <memory>
#include <vector>

class BaseScene;											// ｸﾗｽのﾌﾟﾛﾄﾀｲﾌﾟ宣言

using unipueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unipueBase Update(unipueBase own) = 0;			// どのｼｰﾝにもあるので純粋仮想
};

