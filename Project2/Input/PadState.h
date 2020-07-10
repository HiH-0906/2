#pragma once
#include "Input.h"
class PadState :
	public Input
{
public:
	void Update(const PLAYER_ID& p_id)override;
private:
	static keyData _keyCon;
};

