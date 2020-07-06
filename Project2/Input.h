#pragma once
#include<map>
#include<DxLib.h>

enum class INPUT_ID
{
	LEFT = KEY_INPUT_LEFT,
	RIGHT = KEY_INPUT_RIGHT
};

using InputData = std::map<INPUT_ID, std::pair<bool, bool>>;

class Input
{
public:
	Input();
	~Input();
	void Update(void);
	bool GetKeyTrg(INPUT_ID key);
private:
	InputData _input;
	void Init(void);
};

