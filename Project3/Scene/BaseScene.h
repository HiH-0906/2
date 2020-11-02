#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
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
protected:
	int drawScreen_;
};

