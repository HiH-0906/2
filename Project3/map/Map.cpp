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
	mapData_.clear();
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
	ReDrawMap(MapLayer::ITEM);
	oldTime_ = now;
	return true;
}

bool Map::LoadMap(std::string str)
{
	// mapData�ǂݍ���
	std::unique_ptr<Loader::TmxLoader> loadr = std::make_unique<Loader::TmxLoader>(str.c_str());

	info_ = loadr->GetMapInfo();
	int id = 0;

	for (const auto& tmp : loadr->GetmapStr())
	{
		// ���o�����strng�ۑ��p�ꎞ�ϐ�
		std::string mapstr;
		// stream�ɕϊ�
		std::stringstream str(tmp.data);
		// MapInfo�̏������Ƃɕ`��p�X�N���[���쐬
		drawLayer_[tmp.name] = MakeScreen(info_.mapSize.x * info_.chipSize.x, info_.mapSize.x * info_.chipSize.y, true);
		// �A�N�Z�X���₷���悤��enumClass�ɑΉ�����str��ۑ�
		mapKey_[static_cast<MapLayer>(id)] = tmp.name;
		id++;
		// string��int��
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.name].push_back(atoi(mapstr.c_str())-1);
		}
	}
	// �}�b�v�쐬�ɕK�v�ȃf�[�^�̎擾

	// �`��X�N���[���ւ̏����o��
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
	return true;
}

const int& Map::GetDarwMap(MapLayer layer)
{
	// �����o����Ă���X�N���[���Ăяo��
	return drawLayer_[mapKey_[layer]];
}

std::vector<int>& Map::GetMapData(MapLayer layer)
{
	// Map�f�[�^�擾
	return mapData_[mapKey_[layer]];
}
// �w�肳�ꂽlayer����������
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

const ITEM_TYPE Map::CheckHitItem(const Vector2& pos)
{
	auto data = mapData_[mapKey_[MapLayer::ITEM]][pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(info_.mapSize.x)];
	if (data == -1)
	{
		return ITEM_TYPE::NON;
	}
	mapData_[mapKey_[MapLayer::ITEM]][pos.x + static_cast<size_t>(pos.y) * static_cast<size_t>(info_.mapSize.x)] = -1;
	return static_cast<ITEM_TYPE>(data);
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
				// �S�~
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
						if (CheckCircleFlame(Vector2(x, y + 1)) || data.length_.fLength.down == 1)
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

void Map::GeneratoFlame(const Vector2& pos, int length,Time now)
{
	geneList_.push_back(std::make_shared<FlameGenerator>(pos,length,*this, flameData_,now));
}
