#pragma once
#include "Input.h"



class keyState :
	public Input
{
public:
	void Update(const PLAYER_ID& p_id)override;
private:
	static std::map<PLAYER_ID, keyData> _keyCon;
};

