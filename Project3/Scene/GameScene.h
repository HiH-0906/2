#pragma once
#include <memory>
#include <vector>
#include <list>
#include "BaseScene.h"
#include "../map/Map.h"


class Obj;
class Player;

using shared_Obj = std::shared_ptr<Obj>;



class GameScene :
	public BaseScene
{
public:
	uniqueBase Update(uniqueBase own, const Time& now)override;
	void DrawOwnScene(void)override;
	void Init(void)override;
	GameScene();
	~GameScene();

	shared_Obj GetPlayer(int id);
	std::list<shared_Obj> GetObjList(void);
	void SetBomb(Vector2 pos, int& id, int& oid,int length, bool send,std::chrono::system_clock::time_point start);
	
private:
	std::list<shared_Obj> objList_;			// Obj�Ǘ����X�g
	std::shared_ptr<Map> mapMng_;
};

