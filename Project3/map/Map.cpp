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
				data.length_ = {};
			}
		}
	}

	ReDrawMap(MapLayer::WALL);
	oldTime_ = now;
	return true;
}

bool Map::LoadMap(std::string str)
{
	// mapData�ǂݍ���
	Loader::TmxLoader* loadr = new Loader::TmxLoader(str.c_str());

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
	delete loadr;
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
	for (size_t y = 0; y < info_.mapSize.y; y++)
	{
		for (size_t x = 0; x < info_.mapSize.x; x++)
		{
			if (flameData_[x + static_cast<size_t>(y) * static_cast<size_t>(info_.mapSize.x)].endTimeCnt_ != 0)
			{
				const auto& data = flameData_[x + static_cast<size_t>(y) * static_cast<size_t>(info_.mapSize.x)];
				auto frame = data.endTimeCnt_ / (1000 / 6);
				
				frame = abs(frame - 6);
				auto tmp = frame;
				frame %= 4;
				frame = abs(frame - (tmp / 4) * 2) * 3;
				int offset = 0;
				double angle = 0;
				_dbgDrawFormatString(x * static_cast<size_t>(info_.chipSize.x), y * static_cast<size_t>(info_.chipSize.y), 0xffffff, "%d", frame);
				if ((data.length_.fLength.down || data.length_.fLength.up) && (data.length_.fLength.left || data.length_.fLength.right))
				{
					offset = 0;
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 200);
				}
				else if (data.length_.fLength.down || data.length_.fLength.up || data.length_.fLength.left || data.length_.fLength.right)
				{
					const auto RightAngle = DX_PI / 2;
					if (data.length_.fLength.left)
					{
						angle = RightAngle * 2;
					}
					if (data.length_.fLength.down)
					{
						angle = RightAngle;
					}
					if (data.length_.fLength.up)
					{
						angle = RightAngle * 3;
					}
					offset = 1;
					SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
				}
				DrawRotaGraph(x * info_.chipSize.x + (info_.chipSize.x / 2), y * info_.chipSize.y + (info_.chipSize.y / 2), 1.0, angle, lpImageMng.GetID("fire")[frame + offset], true);
			}
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 200);
}

void Map::ResrtOfMap(void)
{
	mapData_.clear();
	drawLayer_.clear();
	mapKey_.clear();
	flameData_.clear();
	info_ = {};
}

void Map::GeneratoFlame(const Vector2& pos, int length,Time now)
{
	geneList_.push_back(std::make_shared<FlameGenerator>(pos,length,*this, flameData_,now));
}
