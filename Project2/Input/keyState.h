#pragma once
#include "Input.h"

// ｷｰﾎﾞｰﾄﾞ入力ｸﾗｽ
class keyState :
	public Input
{
public:
	CON_ID GetID(void)override final;						// 多分継承しない
	void Update(const int& p_id, const int& pad_id)override final;
private:
	static std::map<int, keyData> _keyCon;					// ｷｰｺﾝﾌｨｸﾞ今のところ固定
};

