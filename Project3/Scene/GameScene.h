#pragma once
#include <memory>
#include "BaseScene.h"
#include "../map/Map.h"

class GameScene :
	public BaseScene
{
public:
	uniqueBase Update(uniqueBase own)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
	GameScene();
	~GameScene();
private:
	std::unique_ptr<Map> mapMng_;				// MapŠÖŒW‘å‘Ì‚Á‚Ä‚éƒNƒ‰ƒX
};

