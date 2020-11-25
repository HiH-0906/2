#pragma once
#include <vector>
#include <memory>
#include "../common/Vector2.h"
#include "../Scene/BaseScene.h"

struct FlameLength
{
	int left : 8;
	int right : 8;
	int up : 8;
	int down : 8;
};

union Length
{
	FlameLength fLength;
	int length;
};

struct FlameData
{
	int endTimeCnt_;
	Length length_;
};

class Map;

class FlameGenerator
{
public:
	FlameGenerator(Vector2 chipPos, int length,Map& mapMng, std::vector<FlameData>& data, Time now);
	~FlameGenerator() = default;
	bool Update(const Time& now);
	bool Alive(void);
private:
	Vector2 chipPos_;
	bool alive_;
	bool SetNextFlame(const Time& now);
	int generatTime_;
	Length length_;
	Length olgLength_;
	Time oldTime_;

	std::vector<FlameData>& flameData_;
	Map& mapMng_;
};

