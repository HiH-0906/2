#pragma once
#include <memory>
#include <functional>
#include <Map>
#include "Obj.h"
#include "common/Vector2.h"


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

class Player
	:public Obj
{
public:
	Player(Vector2 pos, Vector2 size ,int speed,int id);
	~Player();
	bool UpdateDef(void)override;
	bool UpdataNet(void);
	void Draw(void)override;
private:
	int id_;
	DIR dir_;
	AnimState state_;
};

