#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include "../TmxLoader/TmxLoader.h"
#include "FlameGenerator.h"

enum class MapLayer
{
	BG,													// �w�i
	ITEM,												// �A�C�e��
	WALL,												// ��
	CHAR,												// �L�����N�^�[�����ʒu �����\���͂��Ȃ�
};

using shared_gene = std::shared_ptr<FlameGenerator>;

class Map
{
public:
	Map();
	~Map();
	bool Update(void);
	bool LoadMap(std::string str);						// TMX����ǂݍ��܂ꂽ�f�[�^���g����MapData�쐬
	const int& GetDarwMap(MapLayer layer);				// �`�悳�ꂽMap�̊l�� �����I�ɉ󂹂�u���b�N������̂ŏ��������Ƃ����K�v�ɂȂ肻��
	std::vector<int>& GetMapData(MapLayer layer);		// �i�[����Ă���}�b�v�f�[�^�̊l��
	void ReDrawMap(MapLayer layer);						// �{���o�[�}�����ă}�b�v����������ȁc���Ă��Ƃō쐬 �����������O�`���߂Ă�������������Ȃ��d���Ȃ����
	bool CheckHitWall(const Vector2& pos);				// �n���ꂽ���W���ǂɓ������Ă��邩�ǂ���
	Vector2 ChengeChip(const Vector2& pos);				// �n���ꂽ���W���`�b�v��
	const Vector2& GetChipSize(void)const;
	const Vector2& GetMapSize(void)const;
	const std::vector<int>& GetFlameData(void)const;
	void ResrtOfMap(void);
	/// <summary>
	/// �����̍��W�ʒu�ɉ��𐶐�����Generator��ݒu
	/// </summary>
	/// <param name="pos">���W</param>
	void GeneratoFlame(const Vector2& pos,int length);
	void GeneratoFlame(const Vector2& pos, int length,FlameDIR dir);
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	std::vector<int> flameData_;						// �����}�b�v�f�[�^
	std::list<shared_gene> geneList_;
	mapInfo info_;
};

