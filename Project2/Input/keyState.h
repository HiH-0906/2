#pragma once
#include "Input.h"



class keyState :
	public Input
{
public:
	CON_ID GetID(void)override final;
	void Update(const int& p_id)override final;
private:
	static std::map<int, keyData> _keyCon;
};

