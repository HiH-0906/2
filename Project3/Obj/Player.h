#pragma once
#include <memory>
#include <functional>
#include <Map>
#include "Obj.h"
#include "../Input/Input.h"
#include "../common/Vector2.h"
#include "../State/DIR.h"
#include "../Scene/GameScene.h"

enum class AnimState
{
	IDEL,
	WALK,
	DETH,
	MAX
};


class Map;

#define UNIT_ID_BASE 5

using moveFunc = std::function<bool(bool, std::list<shared_Obj>)>;

class Player
	:public Obj
{
public:
	Player(Vector2 pos, Vector2 size, Vector2 ImageSize,int speed,int id, std::shared_ptr<Map> mapMng, BaseScene& scene);
	~Player();
	bool UpdateAuto(const Time& now);
	bool UpdateDef(const Time& now)override;
	bool UpdataNet(const Time& now);
	bool CheckHitObj(const shared_Obj& objB, DIR dir);
	std::list<shared_Obj> GetHitObjList(void);
	void Draw(void)override;
	void StockBomb(int id);
	int UseBomb(void);
	static int fallCnt_;
private:
	void FuncInit(void);
	int length_;
	int idScreen_;
	sendData data[4];
	std::map<DIR, Vector2> speedVec_;
	Vector2 chipSize_;
	DIR dir_;
	AnimState state_;
	std::unique_ptr<Input> input_;
	std::list<int> bombList_;
	std::list<moveFunc> moveFunc_;
};

