#include <iostream>
#include <fstream>
#include <sstream>
#include <DxLib.h>
#include "Map.h"
#include "../Parser/TMXParser-master/include/TMXParser.h"
#include "../Parser/TSXParser-master/include/TSXParser.h"

Map::Map()
{
}

Map::~Map()
{
}

bool Map::LoadMap(void)
{
	// mapData�ǂݍ���
	TMX::Parser test("mapData/map.tmx");
	int id = 0;

	int image[12];

	LoadDivGraph("Image/map.png", 12, 4, 3, 32, 32, image, true);

	for (auto tmp : test.tileLayer)
	{
		// ���o�����strng�ۑ��p�ꎞ�ϐ�
		std::string mapstr;
		// stream�ɕϊ�
		std::stringstream str(tmp.second.data.contents);
		// MapInfo�̏������Ƃɕ`��p�X�N���[���쐬
		drawLayer_[tmp.first] = MakeScreen(test.mapInfo.width * test.mapInfo.tileWidth, test.mapInfo.height * test.mapInfo.tileHeight, true);
		// �A�N�Z�X���₷���悤��enumClass�ɑΉ�����str��ۑ�
		mapKey_[static_cast<MapLayer>(id)] = tmp.first;
		id++;
		// string��int��
		while (std::getline(str, mapstr, ',')) {
			mapData_[tmp.first].push_back(atoi(mapstr.c_str()));
		}
	}
	// �`��X�N���[���ւ̏����o��
	for (auto id : mapKey_)
	{
		SetDrawScreen(drawLayer_[id.second]);
		for (unsigned int y = 0; y < test.mapInfo.height; y++)
		{
			for (unsigned int x = 0; x < test.mapInfo.width; x++)
			{
				DrawGraph(x * test.mapInfo.tileWidth, y * test.mapInfo.tileHeight, image[mapData_[id.second][x + y * test.mapInfo.width] - 1], true);
			}
		}
	}
	return true;
}

int& Map::GetDarwMap(MapLayer layer)
{
	// �����o����Ă���X�N���[���Ăяo��
	return drawLayer_[mapKey_[layer]];
}

std::vector<int>& Map::GetMapData(MapLayer layer)
{
	// Map�f�[�^�擾
	return mapData_[mapKey_[layer]];
}

