#pragma once
#include <memory>
#include <string>
#include <array>
#include <vector>
#include "../common/Vector2.h"

class BaseScene;											// ¸×½‚ÌÌßÛÄÀ²ÌßéŒ¾

using unipueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unipueBase Update(unipueBase own) = 0;			// ‚Ç‚Ì¼°İ‚É‚à‚ ‚é‚Ì‚Åƒˆ‰¼‘z
	virtual void Draw(void) = 0;
	virtual void Init(void) = 0;
};

