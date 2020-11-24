#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>
#include "Map.h"
#include "../common/ImageMng.h"


Map::Map()
{
}

Map::~Map()
{
}

bool Map::Update(void)
{
	for (auto& gene : geneList_)
	{
		gene->Update();
	}
	for (auto& data : flameData_)
	{
		if (data > 0)
		{
			data--;
		}
	}
	auto delItr = std::remove_if(geneList_.begin(), geneList_.end(), [](shared_gene& gene) {return !gene->Alive(); });
	geneList_.erase(delItr, geneList_.end());
	ReDrawMap(MapLayer::WALL);
	return true;
}

bool Map::LoadMap(std::string str)
{
	// mapData読み込み
	Loader::TmxLoader* loadr = new Loader::TmxLoader(str.c_str());

	info_ = loadr->GetMapInfo();
	int id = 0;

	for (auto tmp : loadr->GetmapStr())
	{
		// 取り出されるstrng保存用一時変数
		std::string mapstr;
		// streamに変換
		std::stringstream str(tmp.data);
		// MapInfoの情報をもとに描画用スクリーン作成
		drawLayer_[tmp.name] = MakeScreen(info_.mapSize.x * info_.chipSize.x, info_.mapSize.x * info_.chipSize.y, true);
		// アクセスしやすいようにenumClassに対応したstrを保存
		mapKey_[static_cast<MapLayer>(id)] = tmp.name;
		id++;
		// stringをintに
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.name].push_back(atoi(mapstr.c_str())-1);
		}
	}
	// マップ作成に必要なデータの取得

	// 描画スクリーンへの書き出し
	for (auto id : mapKey_)
	{
		SetDrawScreen(drawLayer_[id.second]);
		for (__int64 y = 0; y < info_.mapSize.y; y++)
		{
			for (__int64 x = 0; x < info_.mapSize.x; x++)
			{
				if (mapData_[id.second][x + y * info_.mapSize.x] >= 0)
				{
					DrawGraph(x * info_.chipSize.x, y * info_.chipSize.y, lpImageMng.GetID(loadr->GetMapKey())[mapData_[id.second][x + y * info_.mapSize.x]], true);
				}
			}
		}
	}
	flameData_.resize(static_cast<size_t>(info_.mapSize.x) * info_.mapSize.y);
	delete loadr;
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
// 指定されたlayerを書き換え
void Map::ReDrawMap(MapLayer layer)
{
	SetDrawScreen(drawLayer_[mapKey_[layer]]);
	for (__int64 y = 0; y < info_.mapSize.y; y++)
	{
		for (__int64 x = 0; x < info_.mapSize.x; x++)
		{
			if (mapData_[mapKey_[layer]][x + y * info_.mapSize.x] >= 0)
			{
				DrawGraph(x * info_.chipSize.x, y * info_.chipSize.y, lpImageMng.GetID(info_.key)[mapData_[mapKey_[layer]][x + y * info_.mapSize.x]], true);
			}
		}
	}
}

bool Map::CheckHitWall(const Vector2& pos)
{	
	return (mapData_[mapKey_[MapLayer::WALL]][pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(info_.mapSize.x)] != -1);
}

Vector2 Map::ChengeChip(const Vector2& pos)
{
	Vector2 chip = { pos.x / info_.chipSize.x,pos.y / info_.chipSize.y };
	return std::move(chip);
}

const Vector2& Map::GetChipSize(void)const
{
	return info_.chipSize;
}

const Vector2& Map::GetMapSize(void) const
{
	return info_.mapSize;
}

const std::vector<int>& Map::GetFlameData(void) const
{
	return flameData_;
}

void Map::ResrtOfMap(void)
{
	mapData_.clear();
	drawLayer_.clear();
	mapKey_.clear();
	flameData_.clear();
	info_ = {};
}

void Map::GeneratoFlame(const Vector2& pos, int length)
{
	geneList_.push_back(std::make_shared<FlameGenerator>(pos, FlameDIR{ 1,1,1,1 }, length, 70, *this, flameData_));
}

void Map::GeneratoFlame(const Vector2& pos, int length, FlameDIR dir)
{
	geneList_.push_back(std::make_shared<FlameGenerator>(pos, dir, length, 70, *this, flameData_));
}

