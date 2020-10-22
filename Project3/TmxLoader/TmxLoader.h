#pragma once

#include <string>
#include <vector>
#include <map>
#include "../common/Vector2.h"
#include "../RapidXml/rapidxml.hpp"

// データ格納構造体
struct Layer
{
	std::string name;
	std::string data;
};

// マップ作成時必要情報構造体
struct mapInfo
{
	Vector2 mapSize = Vector2{};
	Vector2 chipSize = Vector2{};
	int allNum = 0;
	int columnsNum = 0;
	int lineNum = 0;
	std::string key = std::string{};
};
// そんな長くないけどusingしとくか感
using mapStr = std::vector<Layer>;

namespace Loader
{
	class TmxLoader
	{
	public:
		TmxLoader();
		TmxLoader(const char* filename);
		bool TmxLoad(std::string filename);			// Tmxファイルロード用 内部でTsxLoadを呼んでいる
		const mapStr GetmapStr(void);				// ロードしてきたstringを渡す奴
		const std::string GetMapKey(void);			// Tsxロード時作成されたImageへアクセスするためのキー
		const mapInfo GetMapInfo(void);				// マップ作成時に必要な情報セット
		~TmxLoader();
	private:
		void VersionMap(void);						// 対応バージョン格納するためだけの奴
		int  GetLayerSize(void);					// レイヤーの最大数獲得するだけの奴
		bool TsxLoad(std::string filename);			// Tsxファイルをロード TmxLoadから呼び出される

		rapidxml::xml_document<> doc_;
		rapidxml::xml_node<>* orign_node_;			// 一番最初のノード 基本的にここからアクセスする

		std::map<std::string, int> version_;		// 今後別バージョンに対応した際こいつに対応したバージョンをキーにして適当な値を入れ入れとけば対応してるかどうか確認するときに楽
		mapStr mapStr_;								// 読み込んだstringの保存場所
		mapInfo info_;								// 読み込んだマップ作製用情報を使いやすい形に変え格納したもの
	};
}

