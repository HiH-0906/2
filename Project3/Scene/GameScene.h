#pragma once
#include <memory>
#include <vector>
#include <functional>
#include <list>
#include "BaseScene.h"
#include "../map/Map.h"


class Obj;
class Player;

using shared_Obj = std::shared_ptr<Obj>;

#define START_CNT 10000

enum class GameState
{
	STY,
	COUNT,
	PLAY
};


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
	
	void initFunc(void);
private:
	std::list<shared_Obj> objList_;			// Objä«óùÉäÉXÉg
	std::shared_ptr<Map> mapMng_;
	std::map<GameState,std::function<void(void)>> cntDownFunc_;
	GameState state_;
};

