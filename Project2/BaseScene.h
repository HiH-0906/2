#pragma once
#include <memory>
#include <vector>

class BaseScene;											// ¸×½‚ÌÌßÛÄÀ²ÌßéŒ¾

using unipueBase = std::unique_ptr<BaseScene>;

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unipueBase Update(unipueBase own) = 0;			// ‚Ç‚Ì¼°İ‚É‚à‚ ‚é‚Ì‚Åƒˆ‰¼‘z
};

