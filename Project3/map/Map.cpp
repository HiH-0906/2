#include <iostream>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "Map.h"
#include "../common/ImageMng.h"

std::unique_ptr<Map, Map::MapDeleter> Map::s_instance(new Map);

Map::Map()
{

}

Map::~Map()
{
}

bool Map::LoadMap(std::string str)
{
	// mapData�ǂݍ���
	Loader::TmxLoader* loadr = new Loader::TmxLoader(str.c_str());

	info_ = loadr->GetMapInfo();
	int id = 0;

	for (auto tmp : loadr->GetmapStr())
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
	auto chip = ChengeChip(pos);
	
	return (mapData_[mapKey_[MapLayer::WALL]][chip.x + static_cast<size_t>(chip.y) * static_cast<size_t>(info_.mapSize.x)] != -1);
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

