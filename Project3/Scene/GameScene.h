#pragma once
#include <memory>
#include <vector>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Player.h"

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
	std::vector<std::shared_ptr<Player>> plList_;			// ネットテスト用Player管理リスト
};

