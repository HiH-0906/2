#pragma once
enum class INPUT_ID
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	ROAT,
	MAX
};

INPUT_ID begin(INPUT_ID);
INPUT_ID end(INPUT_ID);
INPUT_ID operator*(INPUT_ID key);
INPUT_ID operator++(INPUT_ID& key);
