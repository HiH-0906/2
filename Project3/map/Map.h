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
	BG,													// 背景
	ITEM,												// アイテム
	WALL,												// 壁
	CHAR,												// キャラクター初期位置 多分表示はしない
};

// Mapと同じ数字に
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
	bool LoadMap(std::string str);						// TMXから読み込まれたデータを使ってMapData作成
	const int& GetDarwMap(MapLayer layer);				// 描画されたMapの獲得 将来的に壊せるブロックがあるので書き直しとかが必要になりそう
	std::vector<int>& GetMapData(MapLayer layer);		// 格納されているマップデータの獲得
	void ReDrawMap(MapLayer layer);						// ボンバーマンってマップ書き換えるな…ってことで作成 そもそも事前描画やめてもいいかもしれない重くなければ
	bool CheckHitWall(const Vector2& pos);				// 渡された座標が壁に当たっているかどうか
	bool CheckBreakWall(const Vector2& chip);
	bool CheckHitFlame(const Vector2& pos);
	bool CheckCircleFlame(const Vector2 pos);
	bool GetFlameEnd(void);
	const ITEM_TYPE CheckHitItem(const Vector2& pos);	// 自分の位置にアイテムがいるか
	Vector2 ChengeChip(const Vector2& pos);				// 渡された座標をチップに
	void SetFlameData(const Vector2& chip, FlameData data);
	const Vector2& GetChipSize(void)const;
	const Vector2& GetMapSize(void)const;
	const std::vector<FlameData>& GetFlameData(void)const;
	void DrawFlame(void);
	/// <summary>
	/// 引数の座標位置に炎を生成するGeneratorを設置
	/// </summary>
	/// <param name="pos">座標</param>
	void GeneratoFlame(const Vector2& pos,int length, Time now);
private:
	std::map<std::string, std::vector<int>> mapData_;
	std::map<std::string, int> drawLayer_;
	std::map<MapLayer, std::string> mapKey_;
	std::vector<FlameData> flameData_;						// 爆風マップデータ
	std::list<shared_gene> geneList_;
	Time oldTime_;
	bool flameEnd_;
	mapInfo info_;
};

