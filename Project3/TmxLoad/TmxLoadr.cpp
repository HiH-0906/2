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

void Loader::TmxLoadr::VersionMap(void)
{
	version_["1.4.2"] = 1;
}

bool Loader::TmxLoadr::TmxLoad(std::string filename)
{
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	orign_node_ = doc_.first_node("map");


	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		std::cout << "非対応バージョンです。" << std::endl;
		return false;
	}

	info_.mapSize.x = std::atoi(orign_node_->first_attribute("width")->value());
	info_.mapSize.y = std::atoi(orign_node_->first_attribute("height")->value());

	mapStr_.resize(GetLayerSize());

	auto itrLayer = mapStr_.begin();

	for (rapidxml::xml_node<>* layer = orign_node_->first_node("layer"); layer != nullptr; layer = layer->next_sibling())
	{
	
		itrLayer->name = layer->first_attribute("name")->value();
		auto data = layer->first_node("data")->first_node();
		itrLayer->data = data->value();
		++itrLayer;
	}
	std::cout << std::endl;

	auto source = orign_node_->first_node("tileset")->first_attribute("source")->value();

	std::string pass = filename.substr(0, filename.find_last_of("/") + 1);

	return TsxLoad(pass + source);
}

bool Loader::TmxLoadr::TsxLoad(std::string filename)
{
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	orign_node_ = doc_.first_node("tileset");

	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		std::cout << "非対応バージョンです。" << std::endl;
		return false;
	}

	info_.chipSize.x = std::atoi(orign_node_->first_attribute("tilewidth")->value());
	info_.chipSize.y = std::atoi(orign_node_->first_attribute("tileheight")->value());
	info_.allNum = std::atoi(orign_node_->first_attribute("tilecount")->value());
	info_.columnsNum = std::atoi(orign_node_->first_attribute("columns")->value());
	info_.lineNum = info_.allNum / info_.columnsNum;
	info_.key = orign_node_->first_attribute("name")->value();

	auto node = orign_node_->first_node("image");

	std::string source = node->first_attribute("source")->value();

	std::string pass = source.substr(source.find_first_of("/")+1, source.length());

	lpImageMng.GetID(info_.key, pass, { info_.chipSize.x,info_.chipSize.y }, { info_.columnsNum,info_.lineNum });

	return true;
}

int Loader::TmxLoadr::GetLayerSize(void)
{
	if (orign_node_ == nullptr)
	{
		return 0;
	}
	return std::atoi(orign_node_->first_attribute("nextlayerid")->value())-1;
}

mapStr Loader::TmxLoadr::GetmapStr(void)
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
