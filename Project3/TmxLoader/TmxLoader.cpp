#include <iostream>
#include "TmxLoader.h"
#include "../RapidXml/rapidxml.hpp"
#include "../RapidXml/rapidxml_utils.hpp"
#include "../common/ImageMng.h"
#include "../NetWork/NetWork.h"
#include "../_debug/_DebugConOut.h"


Loader::TmxLoader::TmxLoader()
{
	VersionMap();
}

Loader::TmxLoader::TmxLoader(const char* filename)
{
	VersionMap();
	TmxLoad(filename);
}

Loader::TmxLoader::~TmxLoader()
{
}
// �Ή����Ă���o�[�W�����̓o�^
void Loader::TmxLoader::VersionMap(void)
{
	version_["1.4.2"] = 1;
	version_["1.4.3"] = 1;
}
// Tmx�t�@�C�������[�h
bool Loader::TmxLoader::TmxLoad(std::string filename)
{
	// Tmx�֘A����
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	orign_node_ = doc_.first_node("map");

	// �o�[�W�����Ή����Ă��邩�m�F
	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		TRACE("��Ή�TiledVersion��TMX�ł��B" );
		TRACE("�Ή��o�[�W������");
		for (auto ver:version_)
		{
			TRACE(ver.first.c_str());
		}
		TRACE("�ł�");
		return false;
	}
#ifdef _DEBUG
	for (rapidxml::xml_node<>* layer = orign_node_->first_node("layer"); layer != nullptr; layer = layer->next_sibling())
	{
		TRACE(layer->first_attribute("name")->value());
		TRACE(layer->first_node("data")->first_node()->value());
	}
#endif // _DEBUG


	// �}�b�v�̑S�̃T�C�Y�i�[
	info_.mapSize.x = std::atoi(orign_node_->first_attribute("width")->value());
	info_.mapSize.y = std::atoi(orign_node_->first_attribute("height")->value());
	info_.nextLayer = std::atoi(orign_node_->first_attribute("nextlayerid")->value());

	// �f�[�^�̊i�[����
	mapStr_.resize(GetLayerSize());

	auto itrLayer = mapStr_.begin();

	for (rapidxml::xml_node<>* layer = orign_node_->first_node("layer"); layer != nullptr; layer = layer->next_sibling())
	{
		itrLayer->name = layer->first_attribute("name")->value();
		auto data = layer->first_node("data")->first_node();
		itrLayer->data = data->value();
		++itrLayer;
	}

	// Tsx�p�̃p�X�Â���

	auto source = orign_node_->first_node("tileset")->first_attribute("source")->value();

	std::string pass = filename.substr(0, filename.find_last_of("/") + 1);

	return TsxLoad(pass + source);
}

bool Loader::TmxLoader::TsxLoad(std::string filename)
{
	// ����
	rapidxml::file<> file(filename.c_str());
	doc_.parse<0>(file.data());

	// Tmx�Ŋm�F���Ă邯�ǈꉞ�m�F
	if (version_.count(orign_node_->first_attribute("tiledversion")->value()) == 0)
	{
		TRACE("��Ή�TiledVersion��TSM�ł��B");
		TRACE("�Ή��o�[�W������");
		for (auto ver : version_)
		{
			TRACE(ver.first.c_str());
		}
		TRACE("�ł�");
		return false;
	}

	orign_node_ = doc_.first_node("tileset");
	// �}�b�v�֘A �ׂ����� ���ɏȗ����ł��Ȃ��񂶂�Ȃ����Ȋ� ����Ȃ�string��ł��Ȃ��Ă����悤�ɂ�����
	info_.chipSize.x = std::atoi(orign_node_->first_attribute("tilewidth")->value());
	info_.chipSize.y = std::atoi(orign_node_->first_attribute("tileheight")->value());
	info_.allNum = std::atoi(orign_node_->first_attribute("tilecount")->value());
	info_.columnsNum = std::atoi(orign_node_->first_attribute("columns")->value());
	info_.lineNum = info_.allNum / info_.columnsNum;
	info_.key = orign_node_->first_attribute("name")->value();

	// �摜�֘A
	auto node = orign_node_->first_node("image");

	// �p�X�̍쐬
	std::string source = node->first_attribute("source")->value();

	// ����ŏ���/�܂ł�����Tsx�t�@�C����������K�w���ɍs������ʖڂȂ̂ł͋^�f
	std::string pass = source.substr(source.find("Image"));

	lpImageMng.GetID(info_.key, pass, { info_.chipSize.x,info_.chipSize.y }, { info_.columnsNum,info_.lineNum });

	return true;
}

int Loader::TmxLoader::GetLayerSize(void)
{
	if (orign_node_ == nullptr)
	{
		return 0;
	}
	return (std::atoi(orign_node_->first_attribute("nextlayerid")->value()) - 1);
}

const mapStr Loader::TmxLoader::GetmapStr(void)
{
	return mapStr_;
}

const std::string Loader::TmxLoader::GetMapKey(void)
{
	return info_.key;
}

const mapInfo Loader::TmxLoader::GetMapInfo(void)
{
	return info_;
}


