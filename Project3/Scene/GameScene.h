#pragma once
#include <memory>
#include <vector>
#include <list>
#include "BaseScene.h"
#include "../map/Map.h"
#include "../Obj/Player.h"

using shared_Obj = std::shared_ptr<Obj>;

class GameScene :
	public BaseScene
{
public:
	uniqueBase Update(uniqueBase own)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
	GameScene();
	~GameScene();

	shared_Obj GetPlayer(int id);
	void SetBomb(Vector2 pos, int& id, int& oid, bool send,std::chrono::system_clock::time_point start);
	
private:
	std::list<shared_Obj> objList_;			// Objä«óùÉäÉXÉg
	std::shared_ptr<Map> mapMng_;
};

