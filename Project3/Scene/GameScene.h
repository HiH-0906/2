#pragma once
#include <memory>
#include <vector>
#include <list>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Obj/Player.h"

using unique_Obj = std::unique_ptr<Obj>;

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
	std::list<unique_Obj> objList_;			// ネットテスト用Player管理リスト
	std::shared_ptr<Map> mapMng_;
};

