#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <DxLib.h>
#include "Map.h"
#include "../common/ImageMng.h"
#include "../_debug/_DebugDispOut.h"


Map::Map()
{
	flameEnd_ = false;
}

Map::~Map()
{
}

bool Map::Update(const Time& now)
{
	for (auto& gene : geneList_)
	{
		gene->Update(now);
	}
	for (auto& data:flameData_)
	{
		if ((data.endTimeCnt_) > 0)
		{
			data.endTimeCnt_ -= std::chrono::duration_cast<std::chrono::milliseconds>(now - oldTime_).count();
			if (data.endTimeCnt_ < 0)
			{
				data.endTimeCnt_ = 0;
				data.length_.length = 0;
			}
		}
	}

	ReDrawMap(MapLayer::WALL);
	oldTime_ = now;
	return true;
}

bool Map::LoadMap(std::string str)
{
	// mapData読み込み
	Loader::TmxLoader* loadr = new Loader::TmxLoader(str.c_str());

	info_ = loadr->GetMapInfo();
	int id = 0;

	for (const auto& tmp : loadr->GetmapStr())
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
	for (const auto& id : mapKey_)
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
	ClsDrawScreen();
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

bool Map::CheckBreakWall(const Vector2& chip)
{
	if (mapData_[mapKey_[MapLayer::WALL]][chip.x + static_cast<size_t>(chip.y) * static_cast<size_t>(info_.mapSize.x)] == 7)
	{
		mapData_[mapKey_[MapLayer::WALL]][chip.x + static_cast<size_t>(chip.y) * static_cast<size_t>(info_.mapSize.x)] = -1;
		return true;
	}
	return false;
}

bool Map::CheckHitFlame(const Vector2& pos)
{
	return flameData_[pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(info_.mapSize.x)].endTimeCnt_ != 0;
}

bool Map::CheckCircleFlame(const Vector2 pos)
{
	if (CheckHitFlame(pos) || CheckHitWall(pos))
	{
		return false;
	}
	return true;
}

bool Map::GetFlameEnd(void)
{
	return flameEnd_;
}

Vector2 Map::ChengeChip(const Vector2& pos)
{
	Vector2 chip = { pos.x / info_.chipSize.x,pos.y / info_.chipSize.y };
	return std::move(chip);
}

void Map::SetFlameData(const Vector2& chip, FlameData data)
{
	flameData_[chip.x + static_cast<size_t>(chip.y) * static_cast<size_t>(info_.mapSize.x)] = data;
}


const Vector2& Map::GetChipSize(void)const
{
	return info_.chipSize;
}

const Vector2& Map::GetMapSize(void) const
{
	return info_.mapSize;
}

const std::vector<FlameData>& Map::GetFlameData(void) const
{
	return flameData_;
}

void Map::DrawFlame(void)
{
	int cnt = 0;
	for (size_t y = 0; y < info_.mapSize.y; y++)
	{
		for (size_t x = 0; x < info_.mapSize.x; x++)
		{
			if (flameData_[x + static_cast<size_t>(y) * static_cast<size_t>(info_.mapSize.x)].endTimeCnt_ != 0)
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
				const auto& data = flameData_[x + static_cast<size_t>(y) * static_cast<size_t>(info_.mapSize.x)];
				auto frame = data.endTimeCnt_ / (1000 / 6);
				// ゴミ
				frame = abs(frame - 6);
				auto tmp = frame;
				frame %= 4;
				frame = abs(frame - (tmp / 4) * 2) * 3;
				int offset = 0;
				double angle = 0;
				if ((data.length_.fLength.down || data.length_.fLength.up) && (data.length_.fLength.left || data.length_.fLength.right))
				{
					offset = 0;
				}
				else if (data.length_.length != 0)
				{
					const auto RightAngle = DX_PI / 2;
					offset = 1;
					if (data.length_.fLength.left)
					{
						angle = RightAngle * 2;
						if (CheckCircleFlame(Vector2(x - 1, y)) || data.length_.fLength.left == 1)
						{
							offset = 2;
						}
							
					}
					if (data.length_.fLength.down)
					{
						angle = RightAngle;
						if(CheckCircleFlame(Vector2(x , y + 1)) || data.length_.fLength.down == 1)
						{
							offset = 2;
						}
					}
					if (data.length_.fLength.up)
					{
						angle = RightAngle * 3;
						if (CheckCircleFlame(Vector2(x, y - 1)) || data.length_.fLength.up == 1)
						{
							offset = 2;
						}
					}
					if (data.length_.fLength.right)
					{
						if (CheckCircleFlame(Vector2(x + 1, y)) || data.length_.fLength.right == 1)
						{
							offset = 2;
						}
					}
				}
				else
				{
					offset = 1;
				}
				DrawRotaGraph(x * info_.chipSize.x + (info_.chipSize.x / 2), y * info_.chipSize.y + (info_.chipSize.y / 2), 1.0, angle, lpImageMng.GetID("fire")[frame + offset], true);
			}
			else
			{
				cnt++;
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 200);
	if (cnt == flameData_.size())
	{
		flameEnd_ = true;
	}
	else
	{
		flameEnd_ = false;
	}
}

void Map::ResrtOfMap(void)
{
	flameEnd_ = false;
	mapData_.clear();
	drawLayer_.clear();
	mapKey_.clear();
	flameData_.clear();
	geneList_.clear();
	flameEnd_ = false;
	info_ = {};
}

void Map::GeneratoFlame(const Vector2& pos, int length,Time now)
{
	geneList_.push_back(std::make_shared<FlameGenerator>(pos,length,*this, flameData_,now));
}
