#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <chrono>
#include "../common/Vector2.h"

class BaseScene;											// ¸×½‚ÌÌßÛÄÀ²ÌßéŒ¾

using uniqueBase = std::unique_ptr<BaseScene>;
using Time = std::chrono::system_clock::time_point;


using NowTime = std::chrono::system_clock::time_point;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBase Update(uniqueBase own,const Time& now) = 0;			// ‚Ç‚Ì¼°İ‚É‚à‚ ‚é‚Ì‚Åƒˆ‰¼‘z
	virtual void Draw(void);
	virtual void DrawOwnScene(void) = 0;
	virtual void Init(void) = 0;
	void DrawFps(const Time& now);
protected:
	int drawScreen_;
	Time strat_;				// fps‘ª’è—p
	Time end_;
	unsigned int fpsCnt_;
	unsigned int fps_;
	unsigned int secondCnt_;
};

