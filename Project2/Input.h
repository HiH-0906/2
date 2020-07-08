#pragma once
#include<utility>
#include<vector>
#include<map>
#include<DxLib.h>
#include"State/INPUT_ID.h"
#include"State/PLAYER_ID.h"


using InputData = std::map<INPUT_ID, std::pair<bool, bool>>;
using keyPair = std::pair<INPUT_ID, int>;
using keyData = std::vector<keyPair>;

class Input
{
public:
	Input();
	~Input();
	void Update(const PLAYER_ID& p_id);
	bool GetKeyTrg(PLAYER_ID& id,INPUT_ID& key);
private:
	InputData _input;
	void Init(void);
	static std::map<PLAYER_ID, keyData> _keyCon;
};

