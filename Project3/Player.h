#pragma once
#include <memory>
#include <functional>
#include <Map>
#include "common/Vector2.h"


enum class DIR
{
	DOWN,
	LEFT,
	UP,
	RIGHT,
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
{
public:
	Player(Vector2 pos, Vector2 size ,int speed,int id);
	~Player();
	void UpdateH(void);
	void Update(void);
	void UpdataNet(void);
	void Draw(void);
private:
	std::function<void(void)> update_;
	Vector2 pos_;
	Vector2 size_;
	int id_;
	int speed_;
	DIR dir_;
};

