#pragma once
#include "BaseScene.h"
#include "PleyErea.h"

class GameScene:
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	virtual unipueBase Update(unipueBase own) override final;
private:
	std::vector<std::unique_ptr<PleyErea>> playErea_;
};

