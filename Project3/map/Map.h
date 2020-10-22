#pragma once
#include <map>
#include <string>
#include <vector>

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
	Map();
	~Map();

	bool LoadMap(void);									// TMX����ǂݍ��܂ꂽ�f�[�^���g����MapData�쐬
	const int& GetDarwMap(MapLayer layer);				// �`�悳�ꂽMap�̊l��
	std::vector<int>& GetMapData(MapLayer layer);		// �i�[����Ă���}�b�v�f�[�^�̊l��
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
};

