#pragma once
#include <map>
#include <string>
#include <vector>

enum class MapLayer
{
	BG,													// 背景
	ITEM,												// アイテム
	OBJ,												// 壁
	CHAR,												// キャラクター初期位置 多分表示はしない
};

class Map
{
public:
	Map();
	~Map();

	bool LoadMap(void);									// TMXから読み込まれたデータを使ってMapData作成
	const int& GetDarwMap(MapLayer layer);				// 描画されたMapの獲得
	std::vector<int>& GetMapData(MapLayer layer);		// 格納されているマップデータの獲得
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
};

