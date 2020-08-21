#pragma once
#include "BaseScene.h"
class GameOverScene :
	public BaseScene
{
public:
	GameOverScene();
	~GameOverScene();
	unipueBase Update(unipueBase own)override final;
private:
};

