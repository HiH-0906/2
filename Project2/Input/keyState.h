#pragma once
#include "Input.h"

// ·°ÎŞ°ÄŞ“ü—Í¸×½
class keyState :
	public Input
{
public:
	CON_ID GetID(void)override final;						// ‘½•ªŒp³‚µ‚È‚¢
	void Update(const int& p_id)override final;
private:
	static std::map<int, keyData> _keyCon;					// ·°ºİÌ¨¸Ş¡‚Ì‚Æ‚±‚ëŒÅ’è
};

