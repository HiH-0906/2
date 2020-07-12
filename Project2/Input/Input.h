#pragma once
#include<utility>
#include<array>
#include<map>
#include<DxLib.h>
#include"../State/CON_ID.h"
#include"../State/INPUT_ID.h"
#include"../State/PLAYER_ID.h"


using InputData = std::map<INPUT_ID, std::pair<bool, bool>>;

using keyPair = std::pair<INPUT_ID, int>;
using keyData = std::array<keyPair,static_cast<size_t>(INPUT_ID::MAX)>;

class Input
{
public:
	Input();
	virtual void Update(const int& p_id) = 0;
	virtual CON_ID GetID(void) = 0;
	virtual void Setting(void);
	bool GetKeyTrg(INPUT_ID& key);
private:
	void Init(void);
protected:
	InputData _input;
};

