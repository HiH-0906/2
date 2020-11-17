#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include <chrono>
#include "../common/Vector2.h"

class BaseScene;											// �׽���������ߐ錾

using uniqueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual uniqueBase Update(uniqueBase own) = 0;			// �ǂ̼�݂ɂ�����̂ŏ������z
	virtual void Draw(void);
	virtual void DrawOwnScene(void) = 0;
	virtual void Init(void) = 0;
	void DrawFps(void);
protected:
	int drawScreen_;
	std::chrono::system_clock::time_point strat_;				// fps����p
	std::chrono::system_clock::time_point end_;
	unsigned int fpsCnt_;
	unsigned int fps_;
	unsigned int secondCnt_;
};

