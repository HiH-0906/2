#pragma once
#include <type_traits>

enum class DIR
{
	DOWN,
	LEFT,
	RIGHT,
	UP,
	DETH,
	MAX
};

static DIR begin(DIR)
{
	return DIR::DOWN;
}

static DIR operator++(DIR& key)
{
	return (key = static_cast<DIR>(std::underlying_type<DIR>::type(key) + 1));
}

static DIR end(DIR)
{
	return DIR::MAX;
}
static DIR operator*(DIR key)
{
	return key;
}
