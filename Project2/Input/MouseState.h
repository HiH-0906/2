#pragma once
#include<array>
#include<utility>
#include "Input.h"
#include "../common/Vector2.h"

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
	Vector2 _pos;
	Vector2 _buff;
	int _mouseData;
	int _mouseDataOld;
	Vector2 _stanPos;
};