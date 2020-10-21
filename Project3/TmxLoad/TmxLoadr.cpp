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

	node=node->first_node("layer");

	for (rapidxml::xml_node<>* chiled = node; chiled != nullptr; chiled = chiled->next_sibling())
	{
		node = chiled->first_node("data");
		std::cout << node->value() << std::endl;
	}
	std::cout << std::endl;
	return true;
}
