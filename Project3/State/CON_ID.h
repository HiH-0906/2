#pragma once
#include <type_traits>

enum class CON_ID
{
	KEY,				// ·°ÎÞ°ÄÞ
	PAD,				// ¹Þ°ÑÊß¯ÄÞ
	MOUSE,				// Ï³½
	MAX
};

static CON_ID operator++(CON_ID& key) {
	(key = static_cast<CON_ID>(std::underlying_type<CON_ID>::type(key) + 1));
	return key;
};
static CON_ID operator--(CON_ID& key) {
	int tmpkey = (std::underlying_type<CON_ID>::type(key) - 1);
	return key;
};
static CON_ID begin(CON_ID)
{
	return CON_ID::KEY;
};
static CON_ID end(CON_ID)
{
	return CON_ID::MAX;
};
static CON_ID operator*(CON_ID key)
{
	return key;
};
