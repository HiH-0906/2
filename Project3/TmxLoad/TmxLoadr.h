#pragma once

#include <string>
#include <vector>
#include <map>
#include "../common/Vector2.h"
#include "../RapidXml/rapidxml.hpp"

struct Layer
{
	std::string name;
	std::string data;
};

struct mapInfo
{
	Vector2 mapSize;
	Vector2 chipSize;
	int allNum;
	int columnsNum;
	int lineNum;
	std::string key;
};

using mapStr = std::vector<Layer>;

namespace Loader
{
	class TmxLoadr
	{
	public:
		TmxLoadr();
		TmxLoadr(const char* filename);
		bool TmxLoad(std::string filename);
		bool TsxLoad(std::string filename);
		int  GetLayerSize(void);
		mapStr GetmapStr(void);
		const std::string GetMapKey(void);
		const mapInfo GetMapInfo(void);
		~TmxLoadr();
	private:
		void VersionMap(void);						// ‘Î‰ƒo[ƒWƒ‡ƒ“Ši”[‚·‚é‚½‚ß‚¾‚¯‚Ì“z
		rapidxml::xml_document<> doc_;
		rapidxml::xml_node<>* orign_node_;

		std::map<std::string, int> version_;
		mapStr mapStr_;
		mapInfo info_;
	};
}

