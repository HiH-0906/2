#pragma once
#include<array>
#include<utility>
#include "Input.h"
#include "../common/Vector2.h"

// ﾏｳｽ入力ｸﾗｽ 画面の中央(初期値)からどれだけ動いたかで判別わかりずらい
class MouseState
	:public Input
{
public:
	MouseState();
	~MouseState();
	CON_ID GetID(void)override final;
	void Update(const int& p_id)override final;
	void Setting(void)override final;
private:
	bool GetMouseTrg(int id);
	Vector2 _pos;							// ﾏｳｽPos
	Vector2 _buff;							// どれだけ動いたか
	int _mouseData;							// ｸﾘｯｸﾅｳ
	int _mouseDataOld;						// ｸﾘｯｸｵｰﾙﾄﾞ
	Vector2 _stanPos;						// 基準位置 初期値真ん中
};