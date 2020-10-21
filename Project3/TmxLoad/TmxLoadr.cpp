#include <iostream>
#include "TmxLoadr.h"
#include "../RapidXml/rapidxml.hpp"
#include "../RapidXml/rapidxml_utils.hpp"

using namespace Loader;

TmxLoadr::TmxLoadr()
{
}

Loader::TmxLoadr::TmxLoadr(const char* filename)
{
	Load(filename);
}

TmxLoadr::~TmxLoadr()
{
}

bool TmxLoadr::Load(const char* filename)
{
	rapidxml::xml_document<> doc;
	rapidxml::xml_node<>* node;
	rapidxml::file<> file(filename);

	doc.parse<0>(file.data());

	node = doc.first_node("map");

	//for (rapidxml::xml_attribute<>* itr = node->first_attribute(); itr != nullptr; itr = itr->next_attribute())
	//{
	//	std::cout << itr->name() << ":[" << itr->value() << "]" << std::endl;
	//}

	//node = node->first_node("tileset");
	//std::cout << "tileset" << std::endl;
	//for (rapidxml::xml_attribute<>* itr = node->first_attribute(); itr != nullptr; itr = itr->next_attribute())
	//{
	//	std::cout << itr->name() << ":[" << itr->value() << "]" << std::endl;
	//}

	for (rapidxml::xml_node<>* chiled = doc.first_node(); chiled != nullptr; chiled = chiled->next_sibling())
	{
		for (rapidxml::xml_attribute<>* itr = chiled->first_attribute(); itr != nullptr; itr = itr->next_attribute())
		{
			std::cout << itr->name() << ":[" << itr->value() << "]" << std::endl;
		}
	}

	return true;
}
