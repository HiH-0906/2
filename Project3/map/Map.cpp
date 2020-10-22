#include <iostream>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "Map.h"
#include "../TmxLoad/TmxLoadr.h"
#include "../common/ImageMng.h"

Map::Map()
{
}

Map::~Map()
{
}

bool Map::LoadMap(void)
{
	// mapData読み込み

	Loader::TmxLoadr loadr("mapData/map.tmx");

	int id = 0;

	for (auto tmp : loadr.GetmapStr())
	{
		// 取り出されるstrng保存用一時変数
		std::string mapstr;
		// streamに変換
		std::stringstream str(tmp.data);
		// MapInfoの情報をもとに描画用スクリーン作成
		drawLayer_[tmp.name] = MakeScreen(21 * 32, 17 * 32, true);
		// アクセスしやすいようにenumClassに対応したstrを保存
		mapKey_[static_cast<MapLayer>(id)] = tmp.name;
		id++;
		// stringをintに
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.name].push_back(atoi(mapstr.c_str())-1);
		}
	}
	// マップ作成に必要なデータの取得
	auto info = loadr.GetMapInfo();
	// 描画スクリーンへの書き出し
	for (auto id : mapKey_)
	{
		SetDrawScreen(drawLayer_[id.second]);
		for (int y = 0; y < info.mapSize.y; y++)
		{
			for (int x = 0; x < info.mapSize.x; x++)
			{
				if (mapData_[id.second][x + y * info.mapSize.x] >= 0)
				{
					DrawGraph(x * info.chipSize.x, y * info.chipSize.y, lpImageMng.GetID(loadr.GetMapKey())[mapData_[id.second][x + y * info.mapSize.x]], true);
				}
			}
		}
	}
	return true;
}

const int& Map::GetDarwMap(MapLayer layer)
{
	// 書き出されているスクリーン呼び出し
	return drawLayer_[mapKey_[layer]];
}

std::vector<int>& Map::GetMapData(MapLayer layer)
{
	// Mapデータ取得
	return mapData_[mapKey_[layer]];
}

