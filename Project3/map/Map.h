#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <chrono>
#include "../TmxLoader/TmxLoader.h"
#include "FlameGenerator.h"
#include "../Scene/SceneMng.h"

enum class MapLayer
{
	BG,													// �w�i
	ITEM,												// �A�C�e��
	WALL,												// ��
	CHAR,												// �L�����N�^�[�����ʒu �����\���͂��Ȃ�
};

struct FlameDIR
{
	int left : 8;
	int right : 8;
	int up : 8;
	int down : 8;
};

struct Flame
{
	std::chrono::system_clock::time_point time_ = {};
	int endTime_ = {};
	FlameDIR dir_ = {};
};

using FlameData = std::vector<Flame>;

class Map
{
public:
	Map();
	~Map();
	bool Update(NowTime time);
	bool LoadMap(std::string str);						// TMX����ǂݍ��܂ꂽ�f�[�^���g����MapData�쐬
	const int& GetDarwMap(MapLayer layer);				// �`�悳�ꂽMap�̊l�� �����I�ɉ󂹂�u���b�N������̂ŏ��������Ƃ����K�v�ɂȂ肻��
	std::vector<int>& GetMapData(MapLayer layer);		// �i�[����Ă���}�b�v�f�[�^�̊l��
	void ReDrawMap(MapLayer layer);						// �{���o�[�}�����ă}�b�v����������ȁc���Ă��Ƃō쐬 �����������O�`���߂Ă�������������Ȃ��d���Ȃ����
	bool CheckHitWall(const Vector2& pos);				// �n���ꂽ���W���ǂɓ������Ă��邩�ǂ���
	Vector2 ChengeChip(const Vector2& pos);				// �n���ꂽ���W���`�b�v��
	void SetFlame(Vector2 pos, std::chrono::system_clock::time_point time, FlameDIR dir, int endTime);
	void SetGenerator(Vector2 pos,int length,NowTime time,Map& map);
	const Vector2& GetChipSize(void)const;
	const Vector2& GetMapSize(void)const;
	const FlameData& GetFlameData(void)const;
	void ResrtOfMap(void);
private:
	struct FlameGenerator
	{
		FlameGenerator(Vector2 pos, int length, NowTime time, FlameDIR dir, Map& map)
			:pos_(pos),length_(length), geneTime_(1000 / 6), time_(time), map_(map), dir_(dir)
		{
			map_.SetFlame(pos_, time, dir_, geneTime_ * 7);
		};
		~FlameGenerator() = default;
		void Update(NowTime time);
		Vector2 pos_;
		FlameDIR dir_;
		int length_;
		const int geneTime_;
		Map& map_;
		std::chrono::system_clock::time_point time_;
	};
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	FlameData flameData_;						// �����}�b�v�f�[�^
	mapInfo info_;
	std::list<std::unique_ptr<FlameGenerator>> geneList_;
};

