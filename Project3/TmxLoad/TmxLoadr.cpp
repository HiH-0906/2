#include <iostream>
#include "TmxLoadr.h"
#include "../RapidXml/rapidxml.hpp"
#include "../RapidXml/rapidxml_utils.hpp"
#include "../common/ImageMng.h"


Loader::TmxLoadr::TmxLoadr()
{
	VersionMap();
}

Loader::TmxLoadr::TmxLoadr(const char* filename)
{
	VersionMap();
	TmxLoad(filename);
}

Loader::TmxLoadr::~TmxLoadr()
{
}
// 対応しているバージョンの登録
void Loader::TmxLoadr::VersionMap(void)
{
	version_["1.4.2"] = 1;
}
// Tmxファイルをロード
bool Loader::TmxLoadr::TmxLoad(std::string filename)
{
	// Tmx関連準備
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	orign_node_ = doc_.first_node("map");

	// バージョン対応しているか確認
	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		std::cout << "非対応TiledVersionのTMXです。" << std::endl;
		std::cout << "対応バージョンは" << std::endl;
		for (auto ver:version_)
		{
			std::cout << "[" << ver.first << "]" << std::endl;
		}
		std::cout << "です" << std::endl;
		return false;
	}
#ifdef _DEBUG
	for (rapidxml::xml_node<>* layer = orign_node_->first_node("layer"); layer != nullptr; layer = layer->next_sibling())
	{
		std::cout << layer->first_attribute("name")->value() << std::endl;
		std::cout << layer->first_node("data")->first_node()->value() << std::endl;
	}
#endif // _DEBUG


	// マップの全体サイズ格納
	info_.mapSize.x = std::atoi(orign_node_->first_attribute("width")->value());
	info_.mapSize.y = std::atoi(orign_node_->first_attribute("height")->value());

	// データの格納準備
	mapStr_.resize(GetLayerSize());

	auto itrLayer = mapStr_.begin();

	for (rapidxml::xml_node<>* layer = orign_node_->first_node("layer"); layer != nullptr; layer = layer->next_sibling())
	{
		itrLayer->name = layer->first_attribute("name")->value();
		auto data = layer->first_node("data")->first_node();
		itrLayer->data = data->value();
		++itrLayer;
	}

	// Tsx用のパスづくり

	auto source = orign_node_->first_node("tileset")->first_attribute("source")->value();

	std::string pass = filename.substr(0, filename.find_last_of("/") + 1);

	return TsxLoad(pass + source);
}

bool Loader::TmxLoadr::TsxLoad(std::string filename)
{
	// 準備
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	orign_node_ = doc_.first_node("tileset");

	// Tmxで確認してるけど一応確認
	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		std::cout << "非対応TiledVersionのTSMです。" << std::endl;
		std::cout << "対応バージョンは" << std::endl;
		for (auto ver : version_)
		{
			std::cout << "[" << ver.first << "]" << std::endl;
		}
		std::cout << "です" << std::endl;
		return false;
	}

	// マップ関連 べた書き 特に省略もできないんじゃないかな感 やれるならstringを打たなくていいようにしたい
	info_.chipSize.x = std::atoi(orign_node_->first_attribute("tilewidth")->value());
	info_.chipSize.y = std::atoi(orign_node_->first_attribute("tileheight")->value());
	info_.allNum = std::atoi(orign_node_->first_attribute("tilecount")->value());
	info_.columnsNum = std::atoi(orign_node_->first_attribute("columns")->value());
	info_.lineNum = info_.allNum / info_.columnsNum;
	info_.key = orign_node_->first_attribute("name")->value();

	// 画像関連
	auto node = orign_node_->first_node("image");

	// パスの作成
	std::string source = node->first_attribute("source")->value();

	// これ最初の/までだけどTsxファイルがもう一階層下に行ったら駄目なのでは疑惑
	std::string pass = source.substr(source.find_first_of("/Image") + 1, source.length());

	lpImageMng.GetID(info_.key, pass, { info_.chipSize.x,info_.chipSize.y }, { info_.columnsNum,info_.lineNum });

	return true;
}

int Loader::TmxLoadr::GetLayerSize(void)
{
	if (orign_node_ == nullptr)
	{
		return 0;
	}
	return (std::atoi(orign_node_->first_attribute("nextlayerid")->value()) - 1);
}

const mapStr Loader::TmxLoadr::GetmapStr(void)
{
	return mapStr_;
}

const std::string Loader::TmxLoadr::GetMapKey(void)
{
	return info_.key;
}

const mapInfo Loader::TmxLoadr::GetMapInfo(void)
{
	return info_;
}
