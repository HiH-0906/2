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
	// mapData�ǂݍ���

	Loader::TmxLoadr loadr("mapData/map.tmx");

	int id = 0;

	for (auto tmp : loadr.GetmapStr())
	{
		// ���o�����strng�ۑ��p�ꎞ�ϐ�
		std::string mapstr;
		// stream�ɕϊ�
		std::stringstream str(tmp.data);
		// MapInfo�̏������Ƃɕ`��p�X�N���[���쐬
		drawLayer_[tmp.name] = MakeScreen(21 * 32, 17 * 32, true);
		// �A�N�Z�X���₷���悤��enumClass�ɑΉ�����str��ۑ�
		mapKey_[static_cast<MapLayer>(id)] = tmp.name;
		id++;
		// string��int��
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.name].push_back(atoi(mapstr.c_str())-1);
		}
	}
	// �}�b�v�쐬�ɕK�v�ȃf�[�^�̎擾
	auto info = loadr.GetMapInfo();
	// �`��X�N���[���ւ̏����o��
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
	// �����o����Ă���X�N���[���Ăяo��
	return drawLayer_[mapKey_[layer]];
}

std::vector<int>& Map::GetMapData(MapLayer layer)
{
	// Map�f�[�^�擾
	return mapData_[mapKey_[layer]];
}

