#pragma once
#include "Input.h"
class PadState :
	public Input
{
public:
	CON_ID GetID(void)override final;
	void Update(const int& p_id)override final;
private:
	static keyData _keyCon;
};

