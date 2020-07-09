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
	virtual void Update(const PLAYER_ID& p_id) = 0;
	bool GetKeyTrg(PLAYER_ID& id,INPUT_ID& key);
private:
	void Init(void);
protected:
	InputData _input;
	
};

