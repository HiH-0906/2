#pragma once

enum class SCREEN_ID
{
	BG,
	PLAY,
	FRONT,
	MAX
};

// autoforï∂ópä÷êîåQ
static SCREEN_ID begin(SCREEN_ID)
{
	return SCREEN_ID::BG;
};
static SCREEN_ID end(SCREEN_ID)
{
	return SCREEN_ID::MAX;
};
static SCREEN_ID operator*(SCREEN_ID key)
{
	return key;
};
static SCREEN_ID operator++(SCREEN_ID& key)
{
	return (key = static_cast<SCREEN_ID>(std::underlying_type<SCREEN_ID>::type(key) + 1));
};