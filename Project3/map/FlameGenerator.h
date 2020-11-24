#pragma once
#include <vector>
#include <memory>
#include "../common/Vector2.h"

struct FlameDIR
{
	int left : 8;
	int right : 8;
	int up : 8;
	int down : 8;
};

class Map;

class FlameGenerator
{
public:
	FlameGenerator(Vector2 chipPos, FlameDIR dir, int length, int time,Map& mapMng , std::vector<int>& data);
	~FlameGenerator() = default;
	bool Update(void);
	bool Alive(void);
private:
	Vector2 chipPos_;
	FlameDIR flameDir_;
	int length_;
	int time_;
	int cnt_;
	bool alive_;
	bool SetNextFlame(void);

	std::vector<int>& flameData_;
	Map& mapMng_;
};

