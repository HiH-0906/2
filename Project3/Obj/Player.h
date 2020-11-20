#pragma once
#include <memory>
#include <functional>
#include <Map>
#include "Obj.h"
#include "../Input/Input.h"
#include "../common/Vector2.h"

enum class DIR
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
	MAX
};

enum class AnimState
{
	IDEL,
	WALK,
	MAX
};

static DIR begin(DIR)
{
	return DIR::DOWN;
}

static DIR operator++(DIR& key)
{
	return (key = static_cast<DIR>(std::underlying_type<DIR>::type(key) + 1));
}

static DIR end(DIR)
{
	return DIR::MAX;
}
static DIR operator*(DIR key)
{
	return key;
}

class Map;

#define UNIT_ID_BASE 5

using moveFunc = std::function<bool(bool)>;

class Player
	:public Obj
{
public:
	Player(Vector2 pos, Vector2 size, Vector2 ImageSize,int speed,int id, std::shared_ptr<Map> mapMng, BaseScene& scene);
	~Player();
	bool UpdateAuto(void);
	bool UpdateDef(void)override;
	bool UpdataNet(void);
	bool CheckHitWall(DIR dir);
	void Draw(void)override;
	void StockBomb(int id);
	int UseBomb(void);
	static int fallCnt_;
private:
	void FuncInit(void);
	sendData data[4];
	std::map<DIR, Vector2> speedVec_;
	Vector2 chipSize_;
	Vector2 chPos_;
	DIR dir_;
	AnimState state_;
	std::unique_ptr<Input> input_;
	std::list<int> bombList_;
	std::list<moveFunc> moveFunc_;
};

