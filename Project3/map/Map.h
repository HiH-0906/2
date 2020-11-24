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
	BG,													// 背景
	ITEM,												// アイテム
	WALL,												// 壁
	CHAR,												// キャラクター初期位置 多分表示はしない
};

using shared_gene = std::shared_ptr<FlameGenerator>;

class Map
{
public:
	Map();
	~Map();
	bool Update(void);
	bool LoadMap(std::string str);						// TMXから読み込まれたデータを使ってMapData作成
	const int& GetDarwMap(MapLayer layer);				// 描画されたMapの獲得 将来的に壊せるブロックがあるので書き直しとかが必要になりそう
	std::vector<int>& GetMapData(MapLayer layer);		// 格納されているマップデータの獲得
	void ReDrawMap(MapLayer layer);						// ボンバーマンってマップ書き換えるな…ってことで作成 そもそも事前描画やめてもいいかもしれない重くなければ
	bool CheckHitWall(const Vector2& pos);				// 渡された座標が壁に当たっているかどうか
	Vector2 ChengeChip(const Vector2& pos);				// 渡された座標をチップに
	const Vector2& GetChipSize(void)const;
	const Vector2& GetMapSize(void)const;
	const std::vector<int>& GetFlameData(void)const;
	void ResrtOfMap(void);
	/// <summary>
	/// 引数の座標位置に炎を生成するGeneratorを設置
	/// </summary>
	/// <param name="pos">座標</param>
	void GeneratoFlame(const Vector2& pos,int length);
	void GeneratoFlame(const Vector2& pos, int length,FlameDIR dir);
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	std::vector<int> flameData_;						// 爆風マップデータ
	std::list<shared_gene> geneList_;
	mapInfo info_;
};

