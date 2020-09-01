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
	void Update(void)override final;
	void Setting(const int& p_id, const int& pad_id)override final;
	void Draw(Vector2&& pos);
private:
	bool GetMouseTrg(int id);
	int cnt_;
	int rad_;
	Vector2 _pos;							// ﾏｳｽPos
	Vector2 _buff;							// どれだけ動いたか
	int _mouseData;							// ｸﾘｯｸﾅｳ
	int _mouseDataOld;						// ｸﾘｯｸｵｰﾙﾄﾞ
	Vector2 _stanPos;						// 基準位置 初期値真ん中
	int mouseScreen_;
};