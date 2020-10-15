#pragma once
#include "Input.h"

// ºÝÄÛ°×°“ü—Í¸×½(Žè”²‚«)
class PadState :
	public Input
{
public:
	CON_ID GetID(void)override final;
	void Update(void)override final;
	void Setting(const int& p_id, const int& pad_id)override;
private:
	static std::map<int, keyData> _keyCon;
};

