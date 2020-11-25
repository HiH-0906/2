#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <chrono>
#include "../common/Vector2.h"

class BaseScene;											// ｸﾗｽのﾌﾟﾛﾄﾀｲﾌﾟ宣言

using uniqueBase = std::unique_ptr<BaseScene>;
using Time = std::chrono::system_clock::time_point;


using NowTime = std::chrono::system_clock::time_point;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBase Update(uniqueBase own,const Time& now) = 0;			// どのｼｰﾝにもあるので純粋仮想
	virtual void Draw(void);
	virtual void DrawOwnScene(void) = 0;
	virtual void Init(void) = 0;
	void DrawFps(const Time& now);
protected:
	int drawScreen_;
	Time strat_;				// fps測定用
	Time end_;
	unsigned int fpsCnt_;
	unsigned int fps_;
	unsigned int secondCnt_;
};

