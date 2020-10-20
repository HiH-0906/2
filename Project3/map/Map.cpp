#include <iostream>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "Map.h"
#include "../test/TMXParser-master/include/TMXParser.h"
#include "../test/TSXParser-master/include/TSXParser.h"

Map::Map()
{
}

Map::~Map()
{
}

bool Map::LoadMap(void)
{
	// map読み込み
	TMX::Parser test("mapData/map.tmx");
	int id = 0;

	int image[12];

	LoadDivGraph("Image/map.png", 12, 4, 3, 32, 32, image, true);

	for (auto tmp : test.tileLayer)
	{
		std::string mapstr;
		std::stringstream str(tmp.second.data.contents);
		drawLayer_[tmp.first] = MakeScreen(test.mapInfo.width* test.mapInfo.tileWidth, test.mapInfo.height* test.mapInfo.tileHeight, true);
		mapKey_[static_cast<MapLayer>(id)] = tmp.first;
		id++;
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.first].push_back(atoi(mapstr.c_str()));
		}
	}
	// 描画スクリーンへの書き出し
	for (auto id : mapKey_)
	{
		SetDrawScreen(drawLayer_[id.second]);
		for (int y = 0; y < test.mapInfo.height; y++)
		{
			for (int x = 0; x < test.mapInfo.width; x++)
			{
				DrawGraph(x * test.mapInfo.tileWidth, y * test.mapInfo.tileHeight, image[mapData_[id.second][x + y * test.mapInfo.width] - 1], true);
			}
		}
	}
	return true;
}

int& Map::GetDarwMap(MapLayer layer)
{
	return drawLayer_[mapKey_[layer]];
}

std::vector<int>& Map::GetMapData(MapLayer layer)
{
	return mapData_[mapKey_[layer]];
}

