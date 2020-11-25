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
	BG,													// 背景
	ITEM,												// アイテム
	WALL,												// 壁
	CHAR,												// キャラクター初期位置 多分表示はしない
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
	bool LoadMap(std::string str);						// TMXから読み込まれたデータを使ってMapData作成
	const int& GetDarwMap(MapLayer layer);				// 描画されたMapの獲得 将来的に壊せるブロックがあるので書き直しとかが必要になりそう
	std::vector<int>& GetMapData(MapLayer layer);		// 格納されているマップデータの獲得
	void ReDrawMap(MapLayer layer);						// ボンバーマンってマップ書き換えるな…ってことで作成 そもそも事前描画やめてもいいかもしれない重くなければ
	bool CheckHitWall(const Vector2& pos);				// 渡された座標が壁に当たっているかどうか
	Vector2 ChengeChip(const Vector2& pos);				// 渡された座標をチップに
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
	FlameData flameData_;						// 爆風マップデータ
	mapInfo info_;
	std::list<std::unique_ptr<FlameGenerator>> geneList_;
};

