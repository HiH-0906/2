#pragma once
#include <map>
#include <string>
#include <vector>
#include "../TmxLoader/TmxLoader.h"

enum class MapLayer
{
	BG,													// �w�i
	ITEM,												// �A�C�e��
	OBJ,												// ��
	CHAR,												// �L�����N�^�[�����ʒu �����\���͂��Ȃ�
};

class Map
{
public:
	Map(std::string str);
	~Map();
	bool LoadMap(std::string str);						// TMX����ǂݍ��܂ꂽ�f�[�^���g����MapData�쐬
	const int& GetDarwMap(MapLayer layer);				// �`�悳�ꂽMap�̊l�� �����I�ɉ󂹂�u���b�N������̂ŏ��������Ƃ����K�v�ɂȂ肻��
	std::vector<int>& GetMapData(MapLayer layer);		// �i�[����Ă���}�b�v�f�[�^�̊l��
	void ReDrawMap(MapLayer layer);						// �{���o�[�}�����ă}�b�v����������ȁc���Ă��Ƃō쐬 �����������O�`���߂Ă�������������Ȃ��d���Ȃ����
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	mapInfo info_;
};

