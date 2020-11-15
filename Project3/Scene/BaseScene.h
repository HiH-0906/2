#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <chrono>
#include "../common/Vector2.h"

class BaseScene;											// ｸﾗｽのﾌﾟﾛﾄﾀｲﾌﾟ宣言

using uniqueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBase Update(uniqueBase own) = 0;			// どのｼｰﾝにもあるので純粋仮想
	virtual void Draw(void);
	virtual void DrawOwnScene(void) = 0;
	virtual void Init(void) = 0;
	void Fps(void);
protected:
	int drawScreen_;
	std::chrono::system_clock::time_point strat_;				// fps測定用
	std::chrono::system_clock::time_point end_;
	unsigned int fpsCnt_;
	unsigned int fps_;
};

