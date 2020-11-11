#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>
#include "../TmxLoader/TmxLoader.h"

enum class MapLayer
{
	BG,													// 背景
	ITEM,												// アイテム
	WALL,												// 壁
	CHAR,												// キャラクター初期位置 多分表示はしない
};

class Map
{
public:
	static Map& GetInstance(void)
	{
		return *s_instance;
	}
	bool LoadMap(std::string str);						// TMXから読み込まれたデータを使ってMapData作成
	const int& GetDarwMap(MapLayer layer);				// 描画されたMapの獲得 将来的に壊せるブロックがあるので書き直しとかが必要になりそう
	std::vector<int>& GetMapData(MapLayer layer);		// 格納されているマップデータの獲得
	void ReDrawMap(MapLayer layer);						// ボンバーマンってマップ書き換えるな…ってことで作成 そもそも事前描画やめてもいいかもしれない重くなければ
	bool CheckHitWall(const Vector2& pos);				// 渡された座標が壁に当たっているかどうか
	Vector2 ChengeChip(const Vector2& pos);					// 渡された座標をチップに
	const Vector2& GetChipSize(void)const;
private:
	Map();
	~Map();
	struct MapDeleter
	{
		void operator()(Map *map) 
		{ 
			delete map;
		};
	};
	static std::unique_ptr<Map, MapDeleter> s_instance;
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	mapInfo info_;
};

