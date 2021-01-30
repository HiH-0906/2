#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include "../TmxLoader/TmxLoader.h"
#include "FlameGenerator.h"
#include "../Scene/BaseScene.h"

enum class MapLayer
{
	BG,													// �w�i
	ITEM,												// �A�C�e��
	WALL,												// ��
	CHAR,												// �L�����N�^�[�����ʒu �����\���͂��Ȃ�
};

// Map�Ɠ���������
enum class ITEM_TYPE
{
	NON,
	BLAST = 8,
	ADDBOMB = 9,
	REMOTE = 10,
	SPEEDUP = 11,
};

using shared_gene = std::shared_ptr<FlameGenerator>;


class Map
{
public:
	Map();
	~Map();
	bool Update(const Time& now);
	bool LoadMap(std::string str);						// TMX����ǂݍ��܂ꂽ�f�[�^���g����MapData�쐬
	const int& GetDarwMap(MapLayer layer);				// �`�悳�ꂽMap�̊l�� �����I�ɉ󂹂�u���b�N������̂ŏ��������Ƃ����K�v�ɂȂ肻��
	std::vector<int>& GetMapData(MapLayer layer);		// �i�[����Ă���}�b�v�f�[�^�̊l��
	void ReDrawMap(MapLayer layer);						// �{���o�[�}�����ă}�b�v����������ȁc���Ă��Ƃō쐬 �����������O�`���߂Ă�������������Ȃ��d���Ȃ����
	bool CheckHitWall(const Vector2& pos);				// �n���ꂽ���W���ǂɓ������Ă��邩�ǂ���
	bool CheckBreakWall(const Vector2& chip);
	bool CheckHitFlame(const Vector2& pos);
	bool CheckCircleFlame(const Vector2 pos);
	bool GetFlameEnd(void);
	const ITEM_TYPE CheckHitItem(const Vector2& pos);	// �����̈ʒu�ɃA�C�e�������邩
	Vector2 ChengeChip(const Vector2& pos);				// �n���ꂽ���W���`�b�v��
	void SetFlameData(const Vector2& chip, FlameData data);
	const Vector2& GetChipSize(void)const;
	const Vector2& GetMapSize(void)const;
	const std::vector<FlameData>& GetFlameData(void)const;
	void DrawFlame(void);
	/// <summary>
	/// �����̍��W�ʒu�ɉ��𐶐�����Generator��ݒu
	/// </summary>
	/// <param name="pos">���W</param>
	void GeneratoFlame(const Vector2& pos,int length, Time now);
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	std::vector<FlameData> flameData_;						// �����}�b�v�f�[�^
	std::list<shared_gene> geneList_;
	Time oldTime_;
	bool flameEnd_;
	mapInfo info_;
};

