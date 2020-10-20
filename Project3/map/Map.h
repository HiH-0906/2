#pragma once
#include <map>
#include <string>
#include <vector>

enum class MapLayer
{
	BG,
	ITEM,
	OBJ,
	CHAR,
};

class Map
{
public:
	Map();
	~Map();

	bool LoadMap(void);
	int& GetDarwMap(MapLayer layer);
	std::vector<int>& GetMapData(MapLayer layer);
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
};

